import {Component, Input} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";
import {TeacherInfo} from "../../../services/users.service";

@Component({
	selector: "app-info",
	standalone: true,
	imports: [CommonModule, MatCardModule, MatDividerModule],
	templateUrl: "./info.component.html",
	styleUrls: ["./info.component.scss"]
})
export class InfoComponent {
	@Input()
	public teacherInfo?: TeacherInfo;
}
