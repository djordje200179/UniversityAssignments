import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {Notification, UserInfo, UsersService} from "../../services/users.service";
import {MatCardModule} from "@angular/material/card";

@Component({
	selector: "app-notifications",
	standalone: true,
	imports: [CommonModule, MatCardModule],
	templateUrl: "./notifications.component.html",
	styleUrls: ["./notifications.component.scss"]
})
export class NotificationsComponent implements OnInit {
	public userInfo: UserInfo;
	public notifications?: Notification[];

	public constructor(private readonly usersService: UsersService) {
		this.userInfo = usersService.getCurrentUser()!;
	}

	public ngOnInit(): void {
		this.usersService.getNotifications(this.userInfo.username!).subscribe(
			notifications => {
				notifications.reverse();

				console.log(notifications);

				for (const notification of notifications)
					notification.createdAt = new Date(notification.createdAt);

				this.notifications = notifications;
			},
			console.error
		);
	}
}
