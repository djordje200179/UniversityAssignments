import {Component, Inject, Input, OnChanges, OnDestroy, OnInit, SimpleChanges, ViewEncapsulation} from "@angular/core";
import {CommonModule, DOCUMENT} from "@angular/common";
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
import {CalendarDateFormatter, CalendarEvent, CalendarWeekModule} from "angular-calendar";
import {CustomDateFormatter} from "../../classes/teacher-calendar/teacher-calendar.component";
import {addHours, startOfDay} from "date-fns";

@Component({
	selector: "app-schedule-form",
	encapsulation: ViewEncapsulation.None,
	standalone: true,
	imports: [
		CommonModule, MatCardModule, MatInputModule,
		MatDatepickerModule, MatNativeDateModule,
		MatSelectModule, MatCheckboxModule, FormsModule,
		MatButtonModule, CalendarWeekModule
	],
	templateUrl: "./schedule-form.component.html",
	styleUrls: ["./schedule-form.component.scss"],
	providers: [
		{
			provide: CalendarDateFormatter,
			useClass: CustomDateFormatter,
		},
	],
})
export class ScheduleFormComponent implements OnChanges, OnInit, OnDestroy {
	public readonly availableTimes: { hour: number, minute: number }[];

	public viewDate: Date = new Date();

	public events?: CalendarEvent[];

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

	public chosenTime : { hour: number, minute: number } = null!;

	public constructor(
		@Inject(DOCUMENT) private readonly document: Document,
		private readonly usersService: UsersService, private readonly classesService: ClassesService
	) {
		this.scheduleRequest.student = this.usersService.getCurrentUser()!.username!;

		this.availableTimes = [];
		for (let hour = 8; hour < 20; hour++) {
			this.availableTimes.push({hour, minute: 0});
			this.availableTimes.push({hour, minute: 30});
		}
	}

	private readonly colors: any = {
		green: {
			primary: "#00ff00",
			secondary: "#ccffcc"
		},
		yellow: {
			primary: "#e3bc08",
			secondary: "#FDF1BA"
		},
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

			if (newTeacherInfo == undefined) {
				this.scheduleRequest.teacher = "";
				return;
			}

			this.scheduleRequest.teacher = newTeacherInfo.info.username!;

			this.classesService.getAllTeacherClasses(this.scheduleRequest.teacher).subscribe(
				classes => {
					const newEvents = [];

					for (const c of classes) {
						c.time = new Date(c.time);

						if (c.cancelled)
							continue;

						const event: CalendarEvent = {
							start: c.time,
							end: addHours(c.time, c.double ? 2 : 1),
							title: `${c.subject} - ${c.student.info.username!}`,
							color: c.confirmed ? this.colors.green : this.colors.yellow,
						};
						newEvents.push(event);
					}

					this.events = newEvents;
				},
				console.error
			);
		}
    }

	public submit() {
		if (this.scheduleRequest.subject == "") {
			alert("Морате изабрати предмет!");
			return;
		}

		if (!this.chosenTime) {
			alert("Морате изабрати вријеме!");
			return;
		}

		this.scheduleRequest.time.setHours(this.chosenTime.hour, this.chosenTime.minute, 0, 0);
		this.classesService.scheduleClass(this.scheduleRequest).subscribe(
			result => {
				alert("Час је успјешно заказан!")
			},
			err => {
				if (!(err instanceof HttpErrorResponse)) {
					alert("Непозната грешка!");
					console.error(err);
					return;
				}

				if (err.status == 409) {
					alert("Изабрани наставник већ има час у изабраном термину!");
					return;
				}

				if (err.status == 400) {
					alert(err.error);
					return;
				}

				alert("Непозната грешка!");
				console.error(err);
			}
		);
	}

	public ngOnInit(): void {
		this.document.body.classList.add("dark-theme");
	}

	public ngOnDestroy(): void {
		this.document.body.classList.remove("dark-theme");
	}

	public hourClicked(date: Date) {
		const hour = date.getHours();
		const minute = date.getMinutes();

		this.chosenTime = this.availableTimes.find(t => t.hour == hour && t.minute == minute)!;
		this.scheduleRequest.time = startOfDay(date);
	}
}
