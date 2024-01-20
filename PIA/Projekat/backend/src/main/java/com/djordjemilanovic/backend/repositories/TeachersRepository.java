package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.TeacherEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.Collection;

@Repository
public interface TeachersRepository extends JpaRepository<TeacherEntity, String> {
	Collection<TeacherEntity> findAllByTeachesLowerElementaryIsTrueAndActivatedIsTrue();
	Collection<TeacherEntity> findAllByTeachesUpperElementaryIsTrueAndActivatedIsTrue();
	Collection<TeacherEntity> findAllByTeachesHighIsTrueAndActivatedIsTrue();

	long countByActivatedIsTrue();
}
