import {Component, OnInit} from "@angular/core";
import { CommonModule } from '@angular/common';
import {MatButtonModule} from "@angular/material/button";
import {MatTableModule} from "@angular/material/table";
import {UsersService} from "../../../services/users.service";
import {ClassesService, ClassInfo} from "../../../services/classes.service";
import {MatButtonToggleChange, MatButtonToggleModule} from "@angular/material/button-toggle";
import {count} from "rxjs";
import {FormsModule} from "@angular/forms";

enum State {
	Cancelable = 'cancelable',
	Soon = 'not-cancelable',
	Joinable = 'joinable',
}

interface Row {
	id: number;
	studentName: string;
	topic: string;
	subject: string;
	time: Date;
	state: State;
}

@Component({
  selector: 'app-teacher-upcoming',
  standalone: true,
	imports: [CommonModule, MatButtonModule, MatTableModule, MatButtonToggleModule, FormsModule],
  templateUrl: './teacher-upcoming.component.html',
  styleUrls: ['./teacher-upcoming.component.scss']
})
export class TeacherUpcomingComponent implements OnInit {
	private classesData? : ClassInfo[];
	public tableData?: Row[];

	public displayedColumns: string[] = ["topic", "subject", "studentName", "time", "state"];

	public constructor(private readonly usersService: UsersService, private readonly classesService: ClassesService) {

	}

	private static getTableData(classes: ClassInfo[]): Row[] {
		const firstCancellationTime = new Date(new Date().getTime() + 4 * 60 * 60 * 1000);
		const firstJoinTime = new Date(new Date().getTime() + 15 * 60 * 1000);

		const tableData: Row[] = [];
		for (const c of classes) {
			const studentInfo = c.teacher.info;
			const studentName = `${studentInfo.firstName} ${studentInfo.lastName}`;

			let state: State;
			if (c.time.getTime() > firstCancellationTime.getTime())
				state = State.Cancelable;
			else if (c.time.getTime() > firstJoinTime.getTime())
				state = State.Soon;
			else
				state = State.Joinable;

			tableData.push({
				id: c.id,
				studentName: studentName,
				topic: c.topic,
				subject: c.subject,
				time: c.time,
				state: state,
			});
		}

		return tableData;
	}

	public ngOnInit(): void {
		const teacherUsername = this.usersService.getCurrentUser()!.username!;

		this.classesService.getUpcoming(teacherUsername).subscribe(
			classes => {
				for (const c of classes)
					c.time = new Date(c.time);

				classes.sort((a, b) => b.time.getTime() - a.time.getTime());

				this.classesData = classes;
				this.tableData = TeacherUpcomingComponent.getTableData(this.classesData!.slice(0, 5));
			},
			console.error
		);
	}

	public rowsCountChanged(event: MatButtonToggleChange): void {
		switch (event.value) {
		case "top_5":
			this.tableData = TeacherUpcomingComponent.getTableData(this.classesData!.slice(0, 5));
			break;
		case "top_10":
			this.tableData = TeacherUpcomingComponent.getTableData(this.classesData!.slice(0, 10));
			break;
		case "all":
			this.tableData = TeacherUpcomingComponent.getTableData(this.classesData!);
			break;
		}
	}

	public cancelClass(id: number): void {
		const reason = prompt("Који је разлог отказивања часа?");
		if (reason === null)
			return;

		this.classesService.cancelClass(id, reason).subscribe(
			() => {
				alert("Успјешно сте отказали час!");
				window.location.reload();
			}
		);
	}
}
