package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.ClassEntity;
import com.djordjemilanovic.backend.services.ClassesService;
import com.djordjemilanovic.backend.services.TeachersService;
import com.djordjemilanovic.backend.services.UsersService;
import lombok.AllArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;
import java.util.Date;

@RestController
@CrossOrigin(origins = "*")
@RequestMapping("/classes")
@AllArgsConstructor
public class ClassesController {
	private final ClassesService classesService;

	public record ScheduleRequest(
			String student, String teacher,
			String subject, String topic,
			boolean isDouble, Date time
	) {}

	@PostMapping("/schedule")
	public ResponseEntity<String> scheduleClass(@RequestBody ScheduleRequest request) {
		try {
			classesService.scheduleClass(
					request.student, request.teacher,
					request.subject, request.topic,
					request.isDouble, request.time
			);

			return ResponseEntity.ok("OK");
		} catch (TeachersService.TeacherOccupiedException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).body(e.getMessage());
		} catch (IllegalArgumentException e) {
			return ResponseEntity.badRequest().body(e.getMessage());
		}
	}

	@GetMapping("/archive/{username}")
	public Collection<ClassEntity> getArchive(@PathVariable String username) {
		return classesService.getArchive(username);
	}

	@GetMapping("/upcoming/{username}")
	public Collection<ClassEntity> getUpcoming(@PathVariable String username) {
		return classesService.getUpcoming(username);
	}

	@PostMapping("/cancel/{id}")
	public void cancelClass(@PathVariable int id, @RequestBody String reason) {
		classesService.cancelClass(id, reason);
	}

	@PostMapping("/accept/{id}")
	public void acceptClass(@PathVariable int id) {
		classesService.acceptClass(id);
	}

	@GetMapping("/requested/{username}")
	public Collection<ClassEntity> getRequested(@PathVariable String username) {
		return classesService.getRequested(username);
	}

	@GetMapping("/student/{username}/rating")
	public int getStudentRating(@PathVariable String username) {
		return classesService.getStudentRating(username);
	}
}
