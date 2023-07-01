import Index from "./views/Index.vue";
import Search from "./views/Search.vue";

export default [
	{ 
        path: "/",
        name: "index",
        component: Index
    },
    {
        path: "/search/:query",
        name: "search",
        component: Search,
    },
    {
        path: "/artworks",
        name: "artworks",
        component: Index
    },
    {
        path: "/artists",
        name: "artists",
        component: Index
    },
    {
        path: "/profile",
        name: "profile",
        component: Index
    },
    {
        path: "/about",
        name: "about",
        component: Index
    }
];