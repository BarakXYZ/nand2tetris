// Implemented by BarakXYZ 2024
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <optional>

namespace SymbolTable {

    using sTableAlias = std::unordered_map<std::string, std::string>;

    extern void add_symbol(const std::string symbol, size_t address);
    extern size_t parse_variable(const std::string &symbol);
    extern std::optional<std::string> lookup(const sTableAlias &table, const std::string &key);

    extern std::size_t unique_symbols_counter;
    extern std::unordered_map<std::string, std::size_t> symbol_table;

    extern const sTableAlias comp_table;
    extern const sTableAlias dest_table;
    extern const sTableAlias jump_table;

}  // namespace SymbolTable
#endif
