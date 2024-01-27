package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.SubjectEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import com.djordjemilanovic.backend.services.FileStorageService;
import com.djordjemilanovic.backend.services.TeachersService;
import lombok.AllArgsConstructor;
import lombok.SneakyThrows;
import org.springframework.core.io.InputStreamResource;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;
import java.util.Collection;

@RestController
@CrossOrigin(origins = "*")
@RequestMapping("/teachers")
@AllArgsConstructor
public class TeachersController {
	private final TeachersService teachersService;
	private final FileStorageService fileStorageService;

	@GetMapping("/students/{username}")
	public Collection<StudentEntity> getStudents(@PathVariable("username") String username) {
		return teachersService.getStudents(username);
	}

	@GetMapping
	public Collection<TeacherSubjectEntity> getEnrollments(@RequestParam("schoolYear") int schoolYear) {
		return teachersService.getEnrollments(schoolYear);
	}

	@GetMapping("/{username}")
	public Collection<TeacherSubjectEntity> getEnrollments(@PathVariable("username") String username) {
		return teachersService.getEnrollments(username);
	}

	@GetMapping("/subjects")
	public Collection<SubjectEntity> getAllSubjects() {
		return teachersService.getAllSubjects();
	}

	@PutMapping("/block/{username}")
	public TeacherEntity blockTeacher(@PathVariable("username") String username) {
		teachersService.blockTeacher(username);
		return null;
	}

	@PutMapping("/accept/{username}")
	public TeacherEntity acceptTeacher(@PathVariable("username") String username) {
		teachersService.acceptTeacher(username);
		return null;
	}

	@SneakyThrows
	@GetMapping(value="/biography/{username}", produces = MediaType.APPLICATION_PDF_VALUE)
	public @ResponseBody byte[] getBiography(@PathVariable String username) {
		var resource = fileStorageService.loadBiography(username);
		return resource.getInputStream().readAllBytes();
	}

	@PostMapping("/subjects")
	public ResponseEntity<SubjectEntity> addSubject(@RequestBody String name) {
		try {
			var subject = teachersService.addSubject(name);
			return ResponseEntity.ok(subject);
		} catch (TeachersService.SubjectAlreadyExistsException e) {
			return ResponseEntity.status(HttpStatus.CONFLICT).build();
		}
	}

	@GetMapping("/ratings/{username}")
	public Collection<TeachersService.Rating> getRatings(@PathVariable("username") String username) {
		return teachersService.getRatings(username);
	}

	public record UpdateTeacherRequest (
		String[] subjects,
		boolean teachesLowerElementary,
		boolean teachesUpperElementary,
		boolean teachesHigh
	) {}

	@PutMapping("/update/{username}")
	public TeacherEntity updateTeacher(
			@PathVariable("username") String username,
			@RequestBody UpdateTeacherRequest request
	) {
		return teachersService.updateTeacher(
			username,
			request.subjects(),
			request.teachesLowerElementary(),
			request.teachesUpperElementary(),
			request.teachesHigh()
		);
	}
}
