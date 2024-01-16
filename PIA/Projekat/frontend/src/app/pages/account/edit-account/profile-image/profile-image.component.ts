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
}
