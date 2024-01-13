package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.TeacherEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface TeachersRepository extends JpaRepository<TeacherEntity, String> {
}
