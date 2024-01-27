package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.ClassEntity;
import com.djordjemilanovic.backend.models.NotificationEntity;
import com.djordjemilanovic.backend.repositories.ClassesRepository;
import com.djordjemilanovic.backend.repositories.NotificationsRepository;
import com.djordjemilanovic.backend.repositories.UsersInfoRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.util.Collection;
import java.util.Date;

@Service
@AllArgsConstructor
public class ClassesService {
	private final ClassesRepository classesRepository;
	private final UsersInfoRepository usersInfoRepository;
	private final NotificationsRepository notificationsRepository;

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

		if (classesRepository.findAllByTeacherUsername(teacher).stream().anyMatch(c -> {
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

		classesRepository.save(newClass);
	}

	public Collection<ClassEntity> getArchive(String username) {
		return classesRepository.findAllByStudentUsernameAndTimeBefore(username, new Timestamp(new Date().getTime()));
	}

	public Collection<ClassEntity> getUpcoming(String username) {
		if(usersInfoRepository.findById(username).get().getStudent() != null)
			return classesRepository.findAllByStudentUsernameAndTimeAfter(username, new Timestamp(new Date().getTime()));
		else
			return classesRepository.findAllByTeacherUsernameAndTimeAfterAndConfirmedIsTrueAndCancelledIsFalse(username, new Timestamp(new Date().getTime()));
	}

	public void cancelClass(int id, String reason) {
		var classEntity = classesRepository.findById(id).get();

		classEntity.setCancelled(true);
		classEntity.setTeacherComment(reason);

		classesRepository.save(classEntity);

		var sb = new StringBuilder();
		sb.append("Наставник ");
		sb.append(classEntity.getTeacher().getInfo().getUsername());
		sb.append(" је отказао час због следећег разлога: ");
		sb.append(reason);

		notificationsRepository.save(new NotificationEntity(classEntity.getStudent().getInfo(), sb.toString()));
	}

	public void acceptClass(int id) {
		var classEntity = classesRepository.findById(id).get();

		classEntity.setConfirmed(true);

		classesRepository.save(classEntity);

		var sb = new StringBuilder();
		sb.append("Наставник ");
		sb.append(classEntity.getTeacher().getInfo().getUsername());
		sb.append(" је прихватио час.");

		notificationsRepository.save(new NotificationEntity(classEntity.getStudent().getInfo(), sb.toString()));
	}

	public Collection<ClassEntity> getRequested(String username) {
		return classesRepository.findAllByTeacherUsernameAndTimeAfterAndConfirmedIsFalseAndCancelledIsFalse(username, new Timestamp(new Date().getTime()));
	}

	public Collection<ClassEntity> getTeacherClasses(String username) {
		return classesRepository.findAllByTeacherUsername(username);
	}

	public int getStudentRating(String username) {
		var classes = classesRepository.findAllByStudentUsernameAndTimeBefore(username, new Timestamp(System.currentTimeMillis()));

		int sum = 0, count = 0;
		for (var c : classes) {
			if (c.getTeacherRating() == null)
				continue;

			sum += c.getTeacherRating();
			count++;
		}

		if (count < 3)
			return -1;

		return sum / count;
	}
}
