import {
	ChangeDetectionStrategy,
	Component,
	Inject,
	Injectable,
	OnDestroy,
	OnInit,
	ViewEncapsulation
} from "@angular/core";
import {addDays, addHours, setHours, setMinutes, startOfDay} from "date-fns";
import {
	CalendarDateFormatter,
	CalendarDayModule,
	CalendarEvent,
	CalendarMonthModule,
	CalendarWeekModule, DateFormatterParams
} from "angular-calendar";
import {DOCUMENT, formatDate, NgSwitch, NgSwitchCase} from "@angular/common";
import {ClassesService} from "../../../services/classes.service";
import {UsersService} from "../../../services/users.service";

@Injectable()
export class CustomDateFormatter extends CalendarDateFormatter {

	public override dayViewHour({ date, locale }: DateFormatterParams): string {
		return formatDate(date, 'HH:mm', locale!);
	}

	public override weekViewHour({ date, locale }: DateFormatterParams): string {
		return this.dayViewHour({ date, locale });
	}
}

@Component({
	selector: "app-teacher-calendar",
	encapsulation: ViewEncapsulation.None,
	templateUrl: "./teacher-calendar.component.html",
	standalone: true,
	imports: [
		CalendarMonthModule,
		NgSwitch,
		CalendarWeekModule,
		CalendarDayModule,
		NgSwitchCase
	],
	providers: [
		{
			provide: CalendarDateFormatter,
			useClass: CustomDateFormatter,
		},
	],
	styleUrls: ["./teacher-calendar.component.scss"]
})
export class TeacherCalendarComponent implements OnInit, OnDestroy {
	public viewDate: Date = new Date();

	public events?: CalendarEvent[];

	public constructor(
		@Inject(DOCUMENT) private readonly document: Document,
		private readonly classesService: ClassesService, private readonly usersService: UsersService
	) { }

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

	public ngOnInit(): void {
		this.document.body.classList.add("dark-theme");

		const teacherUsername = this.usersService.getCurrentUser()!.username!;

		this.classesService.getAllTeacherClasses(teacherUsername).subscribe(
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

				console.log(this.events);
			},
			console.error
		);
	}

	public ngOnDestroy(): void {
		this.document.body.classList.remove("dark-theme");
	}
}
