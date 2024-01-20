import {Component, Input} from "@angular/core";
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
import {InfoComponent} from "./info/info.component";
import {ProfileImageComponent} from "./profile-image/profile-image.component";

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
		MatDividerModule,
		InfoComponent,
		ProfileImageComponent
	],
	templateUrl: "./edit-account.component.html",
	styleUrls: ["./edit-account.component.scss"]
})
export class EditAccountComponent {
	@Input()
	studentInfo?: StudentInfo;

	@Input()
	teacherInfo?: TeacherInfo;
}
