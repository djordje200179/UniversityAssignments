package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.TeacherEntity;
import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.Collection;

public interface TeacherSubjectsRepository extends JpaRepository<TeacherSubjectEntity, TeacherSubjectEntity.TeacherSubjectID> {
	Collection<TeacherSubjectEntity> findAllByIdTeacherIn(Collection<TeacherEntity> teachers);
	Collection<TeacherSubjectEntity> findAllByIdTeacherUsername(String username);
}
