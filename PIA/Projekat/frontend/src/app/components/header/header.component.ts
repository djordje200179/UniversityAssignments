import {Component, OnInit} from "@angular/core";
import {MatToolbarModule} from "@angular/material/toolbar";
import {MatIconModule} from "@angular/material/icon";
import {MatButtonModule} from "@angular/material/button";
import {RouterLink} from "@angular/router";

@Component({
	selector: "app-header",
	templateUrl: "./header.component.html",
	styleUrls: ["./header.component.scss"],
	standalone: true,
	imports: [
		MatToolbarModule,
		MatIconModule,
		MatButtonModule,
		RouterLink
	]
})
export class HeaderComponent implements OnInit {
	public username? : string;

	public ngOnInit() {
		console.log(localStorage.getItem("username"));
	}
}
