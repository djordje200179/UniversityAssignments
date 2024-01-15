import {Component} from "@angular/core";
import {MatToolbarModule} from "@angular/material/toolbar";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {RouterLink} from "@angular/router";
import {UserInfo, UsersService} from "../../services/users.service";
import {NgIf} from "@angular/common";

@Component({
	selector: "app-header",
	templateUrl: "./header.component.html",
	styleUrls: ["./header.component.scss"],
	standalone: true,
	imports: [
		MatToolbarModule,
		MatIconModule,
		MatButtonModule,
		RouterLink,
		NgIf
	]
})
export class HeaderComponent {
	public currentUser: UserInfo | null;

	public constructor(private readonly usersService: UsersService) {
		this.currentUser = this.usersService.getCurrentUser();
	}

	public signOut() {
		this.usersService.signOut();
		window.location.reload();
	}
}
