package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import com.djordjemilanovic.backend.services.TeachersService;
import lombok.AllArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;
import java.util.Date;

@RestController
@CrossOrigin(origins = "*")
@RequestMapping("/teachers")
@AllArgsConstructor
public class TeachersController {
	private final TeachersService teachersService;

	@GetMapping
	public Collection<TeacherSubjectEntity> getEnrollments(@RequestParam("schoolYear") int schoolYear) {
		return teachersService.getEnrollments(schoolYear);
	}

	@GetMapping("/{username}")
	public Collection<TeacherSubjectEntity> getEnrollments(@PathVariable("username") String username) {
		return teachersService.getEnrollments(username);
	}

	public record ScheduleRequest(
			String student, String teacher,
			String subject, String topic,
			boolean isDouble, Date time
	) {}

	@PostMapping("/schedule")
	public ResponseEntity<String> scheduleClass(@RequestBody ScheduleRequest request) {
		try {
			teachersService.scheduleClass(
					request.student, request.teacher,
					request.subject, request.topic,
					request.isDouble, request.time
			);

			return ResponseEntity.ok().build();
		} catch (TeachersService.TeacherOccupiedException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).body(e.getMessage());
		} catch (IllegalArgumentException e) {
			return ResponseEntity.badRequest().body(e.getMessage());
		}
	}
}
