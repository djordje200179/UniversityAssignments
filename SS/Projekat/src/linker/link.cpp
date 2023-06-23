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
			if (!symbol.global && symbol.type != symbol::type::SECTION) {
				struct symbol new_symbol(symbol);

				new_symbol.name = '#' + new_symbol.name;

				auto translated_section = symbols.find(object_file.symbols[symbol.section].name);
				auto translated_section_index = translated_section - symbols.data();

				if (section_translation.contains(symbol.section))
					new_symbol.value += section_translation[symbol.section];

				new_symbol.section = translated_section_index;

				symbols.push_back(new_symbol);

				continue;
			}

			auto duplicate_symbol = symbols.find(symbol.name);
			if (!duplicate_symbol) {
				struct symbol new_symbol(symbol);

				if (symbol.type == symbol::type::SECTION)
					new_symbol.section = symbols.size();
				else if(symbol.section != SECTION_UNDEF) {
					auto translated_section = symbols.find(object_file.symbols[symbol.section].name);
					auto translated_section_index = translated_section - symbols.data();

					if (section_translation.contains(symbol.section))
						new_symbol.value += section_translation[symbol.section];

					new_symbol.section = translated_section_index;
				}

				symbols.push_back(new_symbol);
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
				} else if (symbol.section != SECTION_UNDEF)
					throw symbol_already_defined_error(symbol.name);
			}			
		}

		for (const auto& section : object_file.sections) {
			auto section_symbol = object_file.symbols.find(section.name);
			auto section_symbol_index = section_symbol - object_file.symbols.data();

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

				struct relocation new_relocation = relocation;

				new_relocation.symbol = translated_symbol_index;
				if (translated_symbol->type == symbol::type::SECTION)
					new_relocation.addend += old_size;
				if (section_translation.contains(section_symbol_index))
					new_relocation.offset += section_translation[section_symbol_index];

				new_section.relocation_table.push_back(new_relocation);
			}
		}
	}

	std::vector<section> sections_vector;
	for (const auto& section : sections)
		sections_vector.push_back(section.second);

	return object_file{ symbols, sections_vector };
}