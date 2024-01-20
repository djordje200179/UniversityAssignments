package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.repositories.StudentsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@AllArgsConstructor
public class StatsService {
	private final StudentsRepository studentsRepository;
	private final TeachersRepository teachersRepository;

	public record Counters(
		long numberOfStudents,
		long numberOfTeachers,

		int numberOfClassesLastWeek,
		int numberOfClassesLastMonth
	) {}

	public Counters getCounters() {
		return new Counters(
			studentsRepository.count(), teachersRepository.countByActivatedIsTrue(),
			10, 20
		);
	}
}
