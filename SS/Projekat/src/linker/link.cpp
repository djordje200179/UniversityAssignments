#include "common/object_file.hpp"
#include "common/utils.hpp"
#include "linker/args.hpp"
#include <vector>
#include <algorithm>
#include <map>

object_file link(std::vector<object_file> object_files) {
	symbol_table symbols;
	std::map<std::string, section> sections;

	for (const auto& object_file : object_files) {
		std::map<size_t, size_t> section_translation;
		
		for (const auto& symbol : object_file.symbols) {
			auto duplicate_symbol = symbols.find(symbol.name);
			if (!duplicate_symbol) {
				symbols.push_back(symbol);

				if (symbol.type == symbol::type::SECTION)
					symbols.back().section = symbols.size() - 1;
				else if(symbol.section == SECTION_UNDEF)
					symbols.back().section = SECTION_UNDEF;
				else {
					auto translated_section = symbols.find(object_file.symbols[symbol.section].name);
					auto translated_section_index = translated_section - symbols.data();

					if (section_translation.contains(symbol.section))
						symbols.back().value += section_translation[symbol.section];

					symbols.back().section = translated_section_index;
				}
			} else {
				if (duplicate_symbol->type == symbol::type::SECTION && symbol.type == symbol::type::SECTION) {
					const auto& duplicated_symbol_section = sections[duplicate_symbol->name];
					section_translation[symbol.section] = duplicated_symbol_section.content.size();
				} else if (duplicate_symbol->section == SECTION_UNDEF) {
					duplicate_symbol->type = symbol.type;
					duplicate_symbol->value = symbol.value;

					if (symbol.type == symbol::type::SECTION)
						duplicate_symbol->section = (duplicate_symbol - symbols.data());
					else {
						auto translated_section = symbols.find(object_file.symbols[symbol.section].name);
						auto translated_section_index = translated_section - symbols.data();

						duplicate_symbol->section = translated_section_index;

						if (section_translation.contains(symbol.section))
							duplicate_symbol->value += section_translation[symbol.section];
					}
				} else if (symbol.section == SECTION_UNDEF)
					continue;
				else
					throw symbol_already_defined_error(symbol.name);
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