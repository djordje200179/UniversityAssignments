import IndexView from "./views/IndexView.vue";
import SearchView from "./views/SearchView.vue";
import AboutView from "./views/AboutView.vue";
import ArtworksView from "./views/ArtworksView.vue";
import ArtistsView from "./views/ArtistsView.vue";
import ProfileView from "./views/ProfileView.vue";
import ArtworkView from "./views/ArtWorkView.vue";

export default [
	{ 
		path: "/",
		name: "index",
		component: IndexView
	},
	{
		path: "/search/:query",
		name: "search",
		component: SearchView
	},
	{
		path: "/artworks/:category",
		name: "artworks",
		component: ArtworksView
	},
	{
		path: "/artwork/:name",
		name: "artwork",
		component: ArtworkView
	},
	{
		path: "/artists/:category",
		name: "artists",
		component: ArtistsView
	},
	{
		path: "/profile",
		name: "profile",
		component: ProfileView
	},
	{
		path: "/about",
		name: "about",
		component: AboutView
	}
];