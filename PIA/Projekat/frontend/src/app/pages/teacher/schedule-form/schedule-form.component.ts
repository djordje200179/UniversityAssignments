import {Component, Input, OnChanges, SimpleChanges} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatCardModule} from "@angular/material/card";
import {MatInputModule} from "@angular/material/input";
import {MatDatepickerModule} from "@angular/material/datepicker";
import {MatNativeDateModule} from "@angular/material/core";
import {MatSelectModule} from "@angular/material/select";
import {MatCheckboxModule} from "@angular/material/checkbox";
import {TeacherInfo, UsersService} from "src/app/services/users.service";
import {FormsModule} from "@angular/forms";
import {MatButtonModule} from "@angular/material/button";
import {ClassesService, ScheduleRequest} from "../../../services/classes.service";
import {HttpErrorResponse} from "@angular/common/http";

@Component({
	selector: "app-schedule-form",
	standalone: true,
	imports: [CommonModule, MatCardModule, MatInputModule, MatDatepickerModule, MatNativeDateModule, MatSelectModule, MatCheckboxModule, FormsModule, MatButtonModule],
	templateUrl: "./schedule-form.component.html",
	styleUrls: ["./schedule-form.component.scss"]
})
export class ScheduleFormComponent implements OnChanges {
	public readonly availableTimes: { hour: number, minute: number }[];

	@Input()
	public teacherInfo?: TeacherInfo;

	@Input()
	public teacherSubjects?: string[];

	public scheduleRequest : ScheduleRequest = {
		student: "",
		teacher: "",
		subject: "",
		topic: "",
		isDouble: false,
		time: new Date()
	}

	public chosenTime : { hour: number, minute: number } = {hour: 8, minute: 0};

	public constructor(private readonly usersService: UsersService, private readonly classesService: ClassesService) {
		this.scheduleRequest.student = this.usersService.getCurrentUser()!.username!;

		this.availableTimes = [];
		for (let hour = 8; hour < 20; hour++) {
			this.availableTimes.push({hour, minute: 0});
			this.availableTimes.push({hour, minute: 30});
		}
	}

	public ngOnChanges(changes: SimpleChanges): void {
		if ("teacherSubjects" in changes) {
			const newSubjects = changes["teacherSubjects"].currentValue as string[] | undefined;

			if (newSubjects == undefined || newSubjects.length == 0)
				this.scheduleRequest.subject = "";
			else if (newSubjects.length == 1)
				this.scheduleRequest.subject = newSubjects[0];
		}

		if ("teacherInfo" in changes) {
			const newTeacherInfo = changes["teacherInfo"].currentValue as TeacherInfo | undefined;

			if (newTeacherInfo == undefined)
				this.scheduleRequest.teacher = "";
			else
				this.scheduleRequest.teacher = newTeacherInfo.info.username!;
		}
    }

	public submit() {
		this.scheduleRequest.time.setHours(this.chosenTime.hour, this.chosenTime.minute, 0, 0);
		this.classesService.scheduleClass(this.scheduleRequest).subscribe(
			result => {
				alert("Час је успјешно заказан!")
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка!");
					console.log(err);
					return;
				}

				if (err.status == 409) {
					alert("Изабрани наставник већ има час у изабраном термину!");
					return;
				}

				if (err.status == 400) {
					alert(err.message);
					return;
				}

				alert("Непозната грешка!");
				console.log(err);
			}
		);
	}
}
