import {Component} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatDialogModule, MatDialogRef} from "@angular/material/dialog";
import {MatInputModule} from "@angular/material/input";
import {MatButtonModule} from "@angular/material/button";
import {FormsModule} from "@angular/forms";
import {MatButtonToggleModule} from "@angular/material/button-toggle";
import {MatIconModule} from "@angular/material/icon";

export interface Rating {
	comment: string;
	rating: number;
}

@Component({
	selector: "app-comment",
	standalone: true,
	imports: [CommonModule, MatDialogModule, MatInputModule, MatButtonModule, FormsModule, MatButtonToggleModule, MatIconModule],
	templateUrl: "./comment.component.html",
	styleUrls: ["./comment.component.scss"],
})
export class CommentComponent {
	public rating: Rating = {
		comment: "",
		rating: 0
	};

	public constructor(public dialogRef: MatDialogRef<CommentComponent, Rating>) {
	}
}
