import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {UsersService} from "../../../services/users.service";
import {ClassesService} from "../../../services/classes.service";
import {MatSortModule} from "@angular/material/sort";
import {MatTableModule} from "@angular/material/table";
import {MatButtonModule} from "@angular/material/button";

enum State {
	Cancelled = 'cancelled',
	Pending = 'pending',
	Confirmed = 'confirmed',
	Ready = 'ready'
}

interface Row {
	teacherName: string;
	topic: string;
	subject: string;
	teacherComment?: string;
	time: Date;

	state: State;
}

@Component({
	selector: "app-student-upcoming",
	standalone: true,
	imports: [CommonModule, MatSortModule, MatTableModule, MatButtonModule],
	templateUrl: "./student-upcoming.component.html",
	styleUrls: ["./student-upcoming.component.scss"]
})
export class StudentUpcomingComponent implements OnInit {
	public tableData?: Row[];

	public displayedColumns: string[] = ["topic", "subject", "teacherName", "time", "status"];


	public constructor(private readonly usersService: UsersService, private readonly classesService: ClassesService) {

	}

	public ngOnInit(): void {
		const studentUsername = this.usersService.getCurrentUser()!.username!;

		this.classesService.getUpcoming(studentUsername).subscribe(
			classes => {
				const firstJoinTime = new Date(new Date().getTime() + 15 * 60 * 1000);

				const tableData: Row[] = [];
				for (const c of classes) {
					c.time = new Date(c.time);

					const teacherInfo = c.teacher.info;
					const teacherName = `${teacherInfo.firstName} ${teacherInfo.lastName}`;

					let state: State;
					if (c.cancelled)
						state = State.Cancelled;
					else if (!c.confirmed)
						state = State.Pending;
					else if (c.time < firstJoinTime)
						state = State.Ready;
					else
						state = State.Confirmed;

					tableData.push({
						teacherName: teacherName,
						topic: c.topic,
						subject: c.subject,
						teacherComment: c.teacherComment || undefined,
						time: c.time,
						state: state
					});
				}

				tableData.sort((a, b) => b.time.getTime() - a.time.getTime());

				this.tableData = tableData;
			},
			console.error
		);
	}
}
