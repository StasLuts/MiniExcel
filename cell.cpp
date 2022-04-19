#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>

//----------------Cell-----------------

Cell::~Cell()
{
	cell_value_.reset();
}

void Cell::Set(std::string text)
{
	if (text.empty())
	{
		cell_value_ = std::make_unique<CellEmptyImpl>();
		return;
	}
	else if (text[0] != FORMULA_SIGN || (text[0] == FORMULA_SIGN && text.size() == 1))
	{
		cell_value_ = std::make_unique<CellTextImpl>(text);
		return;
	}
	else
	{
		std::string sub_str = text.erase(0, 1);

		try
		{
			cell_value_ = std::make_unique<CellFormulaImpl>(sub_str, sheet_);
		}
		catch (...)
		{
			std::string ref = "#REF!";
			throw FormulaException(ref);
		}
		return;
	}
}

void Cell::Clear()
{
	cell_value_ = std::make_unique<CellEmptyImpl>();
}

Cell::Value Cell::GetValue() const
{
	return cell_value_.get()->ImplGetValue();
}

std::string Cell::GetText() const
{
	return cell_value_.get()->ImplGetText();
}

std::vector<Position> Cell::GetReferencedCells() const
{
	return cell_value_.get()->GetReferencedCells();
}

bool Cell::hasCircularDependency(const Cell* main_cell, const Position& pos) const
{
	for (const auto& cell_pos : GetReferencedCells())
	{
		const Cell* ref_cell = dynamic_cast<const Cell*>(sheet_.GetCell(cell_pos));

		if (pos == cell_pos)
		{
			return true;
		}

		if (!ref_cell)
		{
			sheet_.SetCell(cell_pos, "");
			ref_cell = dynamic_cast<const Cell*>(sheet_.GetCell(cell_pos));
		}

		if (main_cell == ref_cell)
		{
			return true;
		}

		if (ref_cell->hasCircularDependency(main_cell, pos))
		{
			return true;
		}
	}
	return false;
}

void Cell::InvalidateCache()
{
	cell_value_->ResetCache();
}

ImplType Cell::GetType() const
{
	return cell_value_.get()->GetType();
}

//----------------private-----------------

//--------------CellImpl------------------

bool Cell::CellImpl::isCacheValid() const
{
	return true;
}

void Cell::CellImpl::ResetCache()
{
	return;
}

//------------------CellEmptyImpl---------------

Cell::CellEmptyImpl::CellEmptyImpl()
{
	empty_ = "";
}

CellInterface::Value Cell::CellEmptyImpl::ImplGetValue() const
{
	return 0.0;
}

std::string Cell::CellEmptyImpl::ImplGetText() const
{
	return empty_;
}
