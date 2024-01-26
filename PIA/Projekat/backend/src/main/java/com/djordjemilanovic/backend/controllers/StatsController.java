package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.UserInfoEntity;
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
	public StatsService.WelcomePageCounters getWelcomePageCounters() {
		return statsService.getWelcomePageCounters();
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

			enrollments.get(teacherFullName).add(subject.getName());
		}

		return enrollments;
	}

	@GetMapping("/subject-age")
	public Map<String, Map<String, Long>> getSubjectAgeCounters() {
		return statsService.getSubjectAgeCounters();
	}

	@GetMapping("/teacher-gender")
	public Map<UserInfoEntity.Gender, Long> getTeacherCountByGender() {
		return statsService.getTeacherCountByGender();
	}

	@GetMapping("/student-gender")
	public Map<UserInfoEntity.Gender, Long> getStudentCountByGender() {
		return statsService.getStudentCountByGender();
	}

	@GetMapping("/classes-per-days")
	public Collection<Long> getClassesPerDays() {
		return statsService.getClassesPerDays();
	}

	@GetMapping("/top-teachers")
	public Collection<StatsService.TopTeacherInfo> getTopTeachers() {
		return statsService.getTopTeachers();
	}

	@GetMapping("/class-statuses")
	public Map<String, Long> getClassStatuses() {
		return statsService.getClassStatuses();
	}

	@GetMapping("/teacher-statuses")
	public Map<String, Long> getTeacherStatuses() {
		return statsService.getTeacherStatuses();
	}
}