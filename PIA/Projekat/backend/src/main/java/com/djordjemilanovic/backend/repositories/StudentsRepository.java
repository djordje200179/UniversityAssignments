package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.StudentEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface StudentsRepository  extends JpaRepository<StudentEntity, String> {
}
