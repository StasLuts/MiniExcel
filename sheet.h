#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <map>
#include <set>

class Sheet : public SheetInterface
{
public:

    ~Sheet() = default;

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

	// Можете дополнить ваш класс нужными полями и методами

    void InvalidateCellsByPos(const Position& pos);
    const std::set<Position> GetDepCellByPos(const Position& pos);

    const std::map<Position, std::set<Position>> GetSheetDepCells() const;

    void AddDependencedCell(const Position& pos_key, const Position& pos_value);

    void DeleteDependencedCell(const Position& pos);

private:
	
    // Можете дополнить ваш класс нужными полями и методами
    std::map<Position, std::set<Position>> sheet_dependenced_cells_;
    std::map<Position, std::unique_ptr<Cell>> sheet_;

    void PositionCorrect(Position pos) const;
};