package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.UserEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public interface UsersRepository extends JpaRepository<UserEntity, String> {
	Optional<UserEntity> findByUsernameAndPasswordHash(String username, String passwordHash);
}
