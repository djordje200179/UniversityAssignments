package com.djordjemilanovic.backend.controllers;

import com.djordjemilanovic.backend.services.UsersService;
import lombok.AllArgsConstructor;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@AllArgsConstructor
@RequestMapping("/debug")
public class DebugController {
	private final UsersService usersService;

	@GetMapping("/ping")
	public String ping() {
		return "pong";
	}
}
