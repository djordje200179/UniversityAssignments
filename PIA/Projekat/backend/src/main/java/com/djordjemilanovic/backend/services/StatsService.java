package com.djordjemilanovic.backend.services;

import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@AllArgsConstructor
public class StatsService {
	public record Counters(
		int numberOfStudents,
		int numberOfTeachers,

		int numberOfClassesLastWeek,
		int numberOfClassesLastMonth
	) {}

	public Counters getCounters() {
		return new Counters(
			50, 100,
			10, 20
		);
	}
}
