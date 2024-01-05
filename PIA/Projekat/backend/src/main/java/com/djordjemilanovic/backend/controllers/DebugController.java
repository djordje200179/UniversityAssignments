package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.services.UsersService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/debug")
public class DebugController {
	private final UsersService usersService;

	public DebugController(UsersService usersService) {
		this.usersService = usersService;
	}

	@GetMapping("/ping")
	public String ping() {
		return "pong";
	}

	@GetMapping("/db-info")
	public String dbInfo() {
		return "Size:" + usersService.getAll().size();
	}
}
