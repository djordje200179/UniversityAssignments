import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {UsersService} from "../../../../services/users.service";

@Component({
	selector: "app-profile-image",
	standalone: true,
	imports: [CommonModule],
	templateUrl: "./profile-image.component.html",
	styleUrls: ["./profile-image.component.scss"]
})
export class ProfileImageComponent {
	private readonly username: string;

	public constructor(private readonly usersService: UsersService) {
		this.username = usersService.getCurrentUser()!.username!;
	}

	public get profileImageURL(): string {
		return this.usersService.getProfileImageURL(this.username);
	}

	public imageClicked(): void {
		const input = document.createElement("input");
		input.type = "file";
		input.setAttribute("accept", "image/png, image/jpeg, image/jpg");
		input.onchange = (event) => {
			if (event.target === null)
				return;

			const files = (event.target as HTMLInputElement).files;
			if (files === null || files.length === 0)
				return;

			this.usersService.updateProfileImage(this.username, files[0]).subscribe(
				() => {
					window.location.reload();
				},
				console.error
			);
		};

		input.click();
	}
}
