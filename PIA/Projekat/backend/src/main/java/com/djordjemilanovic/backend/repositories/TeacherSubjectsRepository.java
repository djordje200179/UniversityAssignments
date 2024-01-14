package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.TeacherSubjectEntity;
import org.springframework.data.jpa.repository.JpaRepository;

public interface TeacherSubjectsRepository extends JpaRepository<TeacherSubjectEntity, TeacherSubjectEntity.TeacherSubjectID> {
}
