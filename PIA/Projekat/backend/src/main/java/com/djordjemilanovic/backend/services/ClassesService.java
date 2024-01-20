package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.ClassEntity;
import com.djordjemilanovic.backend.repositories.ClassRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.util.Collection;
import java.util.Date;

@Service
@AllArgsConstructor
public class ClassesService {
	private final ClassRepository classRepository;

	public void scheduleClass(
			String student, String teacher,
			String subject, String topic,
			boolean isDouble, Date time
	) throws TeachersService.TeacherOccupiedException {
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
			throw new TeachersService.TeacherOccupiedException("Наставник је заузет у то вријеме");

		var newClass = new ClassEntity(
				teacher, student, subject, topic, new Timestamp(time.getTime()), isDouble
		);

		classRepository.save(newClass);
	}

	public Collection<ClassEntity> getArchive(String username) {
		return classRepository.findAllByStudentUsernameAndTimeBefore(username, new Timestamp(new Date().getTime()));
	}

	public Collection<ClassEntity> getUpcoming(String username) {
		return classRepository.findAllByStudentUsernameAndTimeAfter(username, new Timestamp(new Date().getTime()));
	}
}