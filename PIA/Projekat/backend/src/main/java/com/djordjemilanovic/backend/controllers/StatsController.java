package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.services.StatsService;
import com.djordjemilanovic.backend.services.TeachersService;
import lombok.AllArgsConstructor;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

@RestController
@CrossOrigin(origins = "*")
@RequestMapping("/stats")
@AllArgsConstructor
public class StatsController {
	private final StatsService statsService;
	private final TeachersService teachersService;

	@GetMapping("/counters")
	public StatsService.Counters getCounters() {
		return statsService.getCounters();
	}

	@GetMapping("/enrollments")
	public Map<String, Collection<String>> getEnrollments() {
		var enrollments = new HashMap<String, Collection<String>>();

		for (var enrollment : teachersService.getEnrollments()) {
			var teacher = enrollment.getId().getTeacher();
			var subject = enrollment.getId().getSubject();

			var teacherFullName = teacher.getInfo().getFirstName() + " " + teacher.getInfo().getLastName();

			if (!enrollments.containsKey(teacherFullName))
				enrollments.put(teacherFullName, new ArrayList<>());

			enrollments.get(teacherFullName).add(subject);
		}

		return enrollments;
	}
}