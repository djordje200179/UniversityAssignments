package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.ClassEntity;
import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import com.djordjemilanovic.backend.repositories.ClassRepository;
import com.djordjemilanovic.backend.repositories.TeacherSubjectsRepository;
import com.djordjemilanovic.backend.repositories.TeachersRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Date;

@Service
@AllArgsConstructor
public class TeachersService {
	private final TeacherSubjectsRepository teacherSubjectRepository;
	private final TeachersRepository teachersRepository;
	private final ClassRepository classRepository;

	public Collection<TeacherSubjectEntity> getEnrollments() {
		return teacherSubjectRepository.findAllByIdTeacherActivatedIsTrue();
	}

	public Collection<TeacherSubjectEntity> getEnrollments(int schoolYear) {
		Collection<TeacherEntity> teachers;
		if (schoolYear < 5)
			teachers = teachersRepository.findAllByTeachesLowerElementaryIsTrueAndActivatedIsTrue();
		else if (schoolYear < 9)
			teachers = teachersRepository.findAllByTeachesUpperElementaryIsTrueAndActivatedIsTrue();
		else if (schoolYear < 13)
			teachers = teachersRepository.findAllByTeachesHighIsTrueAndActivatedIsTrue();
		else
			throw new IllegalArgumentException("Invalid school year");

		return teacherSubjectRepository.findAllByIdTeacherIn(teachers);
	}

	public Collection<TeacherSubjectEntity> getEnrollments(String username) {
		return teacherSubjectRepository.findAllByIdTeacherUsername(username);
	}

	public static class TeacherOccupiedException extends Exception {
		public TeacherOccupiedException(String message) {
			super(message);
		}
	}


	public void scheduleClass(
		String student, String teacher,
		String subject, String topic,
		boolean isDouble, Date time
	) throws TeacherOccupiedException {
		if (time.getDay() > 5)
			throw new IllegalArgumentException("Час не може бити викендом");

		if (time.before(new Date()))
			throw new IllegalArgumentException("Час не може бити у прошлости");

		var startTime = new Timestamp(time.getTime());

		var duration = 60L * 60 * 1000;
		if (isDouble)
			duration *= 2;

		var endTime = new Timestamp(startTime.getTime() + duration);

		if (classRepository.findAllByTeacherUsername(teacher).stream().anyMatch(c -> {
				var cStartTime = c.getTime();

				if (cStartTime.equals(startTime))
					return true;

				var cDuration = 60L * 60 * 1000;
				if (c.isDouble())
					cDuration *= 2;

				var cEndTime = new Date(cStartTime.getTime() + cDuration);

				if (cEndTime.equals(endTime))
					return true;

				return time.after(cStartTime) && time.before(cEndTime) || endTime.before(cEndTime) && endTime.after(cStartTime);
			}
		))
			throw new TeacherOccupiedException("Наставник је заузет у то вријеме");

		var newClass = new ClassEntity(
			teacher, student, subject, topic, new Timestamp(time.getTime()), isDouble
		);

		classRepository.save(newClass);
	}
}
