package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.ClassEntity;
import org.springframework.data.jpa.repository.JpaRepository;

import java.sql.Timestamp;
import java.util.Collection;

public interface ClassRepository extends JpaRepository<ClassEntity, Integer> {
	Collection<ClassEntity> findAllByTeacherUsername(String username);

	Collection<ClassEntity> findAllByStudentUsernameAndTimeAfter(String username, Timestamp time);
	Collection<ClassEntity> findAllByStudentUsernameAndTimeBefore(String username, Timestamp time);

}
