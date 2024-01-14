package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.services.StatsService;
import com.djordjemilanovic.backend.services.UsersService;
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
	private final UsersService usersService;

	@GetMapping("/counters")
	public StatsService.Counters getCounters() {
		return statsService.getCounters();
	}

	@GetMapping("/enrollments")
	public Map<String, Collection<String>> getEnrollments() {
		var enrollments = new HashMap<String, Collection<String>>();

		for (var enrollment : usersService.getTeacherEnrollments()) {
			var teacher = enrollment.getId().getTeacher();
			var subject = enrollment.getId().getSubject();

			var teacherFullName = teacher.getUserInfo().getFirstName() + " " + teacher.getUserInfo().getLastName();

			if (!enrollments.containsKey(teacherFullName))
				enrollments.put(teacherFullName, new ArrayList<>());

			enrollments.get(teacherFullName).add(subject);
		}

		return enrollments;
	}
}