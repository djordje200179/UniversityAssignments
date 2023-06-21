#include "common/object_file.hpp"
#include "linker/args.hpp"
#include <vector>
#include <algorithm>
#include <map>

object_file link(std::vector<object_file> object_files) {
	symbol_table symbols;
	std::map<std::string, section> sections;

	for (const auto& object_file : object_files) {
		for (const auto& symbol : object_file.symbols) {
			auto duplicate_symbol = symbols.find(symbol.name);
			if (!duplicate_symbol) {
				symbols.push_back(symbol);

				if (symbol.type == symbol::type::SECTION)
					symbols.back().section = symbols.size() - 1;
				else if(symbol.section == SECTION_UNDEF)
					symbols.back().section = SECTION_UNDEF;
				else {
					auto original_section_name = object_file.symbols[symbol.section].name;
					auto translated_section = symbols.find(original_section_name);
					auto translated_section_index = translated_section - symbols.data();

					symbols.back().section = translated_section_index;
				}
				continue;
			} else {
				if (duplicate_symbol->type == symbol::type::SECTION && symbol.type == symbol::type::SECTION)
					duplicate_symbol->value += symbol.value;
				else if (duplicate_symbol->section == SECTION_UNDEF) {
					duplicate_symbol->type = symbol.type;
					duplicate_symbol->value = symbol.value;

					if (symbol.type == symbol::type::SECTION)
						duplicate_symbol->section = (duplicate_symbol - symbols.data());
					else {
						auto original_section_name = object_file.symbols[symbol.section].name;
						auto translated_section = symbols.find(original_section_name);
						duplicate_symbol->section = translated_section - symbols.data();
					}
				} else if (symbol.section == SECTION_UNDEF)
					continue;
				else
					// TODO: better error handling
					throw std::runtime_error("duplicate symbol: " + symbol.name);
			}			
		}

		for (const auto& section : object_file.sections) {
			auto duplicate_section_it = sections.find(section.name);
			auto& new_section = 
				duplicate_section_it == sections.end() ? 
				(sections[section.name] = section) : 
				duplicate_section_it->second;
			
			size_t old_size;
			if (duplicate_section_it == sections.end()) {
				old_size = 0;
				new_section.relocation_table.clear();
			} else {
				old_size = new_section.content.size();
				new_section.content.insert(
					new_section.content.end(), 
					section.content.begin(), section.content.end()
				);
			}

			for (const auto& relocation : section.relocation_table) {
				auto original_symbol_name = object_file.symbols[relocation.symbol].name;
				auto translated_symbol = symbols.find(original_symbol_name);
				auto translated_symbol_index = translated_symbol - symbols.data();

				auto new_relocation = relocation;
				new_relocation.symbol = translated_symbol_index;

				if (translated_symbol->type == symbol::type::SECTION)
					new_relocation.addend += old_size;

				new_section.relocation_table.push_back(new_relocation);
			}
		}
	}

	std::vector<section> sections_vector;
	for (const auto& section : sections)
		sections_vector.push_back(section.second);

	return object_file{ symbols, sections_vector };
}