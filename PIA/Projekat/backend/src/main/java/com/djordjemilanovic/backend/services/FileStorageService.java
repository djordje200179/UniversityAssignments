package com.djordjemilanovic.backend.services;

import org.springframework.core.io.Resource;
import org.springframework.core.io.UrlResource;
import org.springframework.stereotype.Service;
import org.springframework.util.FileSystemUtils;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

@Service
public class FileStorageService {
	private final Path biographiesRoot = Paths.get("uploads/biographies");
	private final Path profileImagesRoot = Paths.get("uploads/profile-images");

	public void init() throws IOException {
		Files.createDirectories(biographiesRoot);
		Files.createDirectories(profileImagesRoot);
	}

	public void saveBiography(MultipartFile file, String username) throws IOException {
		Files.copy(file.getInputStream(), this.biographiesRoot.resolve(username + ".pdf"));
	}

	public void saveProfileImage(MultipartFile file, String username) throws IOException {
		Files.copy(file.getInputStream(), this.profileImagesRoot.resolve(username + ".png"));
	}

	public void saveGenericProfileImage(String username) throws IOException {
		var resource = new UrlResource(new File("uploads/generic-image.png").toURI());
		Files.copy(resource.getInputStream(), this.profileImagesRoot.resolve(username + ".png"));
	}

	public Resource loadBiography(String username) throws MalformedURLException {
		return new UrlResource(biographiesRoot.resolve(username + ".pdf").toUri());
	}

	public Resource loadProfileImage(String username) throws MalformedURLException {
		return new UrlResource(profileImagesRoot.resolve(username + ".png").toUri());
	}

	public void deleteAll() {
		FileSystemUtils.deleteRecursively(biographiesRoot.toFile());
		FileSystemUtils.deleteRecursively(profileImagesRoot.toFile());
	}
}