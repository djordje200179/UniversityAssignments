import {Component, Input, OnInit} from "@angular/core";
import {CommonModule, NgOptimizedImage} from "@angular/common";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";
import {TeacherInfo, UsersService} from "../../../services/users.service";

@Component({
	selector: "app-info",
	standalone: true,
	imports: [CommonModule, MatCardModule, MatDividerModule, NgOptimizedImage],
	templateUrl: "./info.component.html",
	styleUrls: ["./info.component.scss"]
})
export class InfoComponent {
	@Input()
	public teacherInfo?: TeacherInfo;

	public constructor(private readonly usersService: UsersService) {

	}

	public get teacherProfileImageURL(): string {
		if (this.teacherInfo === undefined)
			return "";

		return this.usersService.getProfileImageURL(this.teacherInfo.info.username!);
	}
}
