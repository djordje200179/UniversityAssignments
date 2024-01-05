package com.djordjemilanovic.backend.services;

import com.djordjemilanovic.backend.models.UserEntity;
import com.djordjemilanovic.backend.repositories.UsersRepository;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class UsersService {
	private final UsersRepository usersRepository;

	public UsersService(UsersRepository usersRepository) {
		this.usersRepository = usersRepository;
	}

	public List<UserEntity> getAll() {
		return usersRepository.findAll();
	}
}
