import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatButtonToggleModule} from "@angular/material/button-toggle";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatInputModule} from "@angular/material/input";
import {MatOptionModule} from "@angular/material/core";
import {MatSelectModule} from "@angular/material/select";
import {FormsModule, ReactiveFormsModule} from "@angular/forms";
import {StudentInfo, TeacherInfo, UserInfo, UsersService} from "../../../services/users.service";
import {MatButtonModule} from "@angular/material/button";
import {MatIconModule} from "@angular/material/icon";
import {MatCardModule} from "@angular/material/card";
import {MatDividerModule} from "@angular/material/divider";

@Component({
	selector: "app-edit-form",
	standalone: true,
	imports: [
		CommonModule,
		MatButtonToggleModule,
		MatFormFieldModule,
		MatInputModule,
		MatOptionModule,
		MatSelectModule,
		ReactiveFormsModule,
		FormsModule,
		MatButtonModule,
		MatIconModule,
		MatCardModule,
		MatDividerModule
	],
	templateUrl: "./edit-account.component.html",
	styleUrls: ["./edit-account.component.scss"]
})
export class EditAccountComponent {
	public userInfo: UserInfo;
	public studentInfo: StudentInfo;
	public teacherInfo: TeacherInfo;

	public constructor(private readonly usersService: UsersService) {
		this.userInfo = usersService.getCurrentUser()!;
		this.studentInfo = usersService.getCurrentUserStudentInfo() ?? {} as StudentInfo;
		this.teacherInfo = usersService.getCurrentUserTeacherInfo() ?? {} as TeacherInfo;
	}
}
