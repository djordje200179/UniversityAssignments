<template>
	<div class="d-flex">
		<div class="dropdown ms-auto mt-1 mb-3 me-3">
			<button class="btn btn-light dropdown-toggle mt-1" type="button" data-bs-toggle="dropdown">
    			{{ $t("sortBy") }}
			</button>
			
			<ul class="dropdown-menu">
				<li>
					<button class="dropdown-item" type="button" @click="sortArtistsByNameAscending">
						{{ $t("artistNameAscending") }}
					</button>
				</li>
				<li>
					<button class="dropdown-item" type="button" @click="sortArtistsByNameDescending">
						{{ $t("artistNameDescending") }}
					</button>
				</li>
			</ul>
		</div>
	</div>

	<div class="container">
		<Artist v-for="artist in artists" :key="artist.name" :artist="artist" />
	</div>
</template>

<script>
import artists from "../artists.json";
import Artist from "../components/Artist.vue";

export default {
	name: "ArtistsView",
	components: {
		Artist
	},
	data() {
		return {
			artists: artists.filter(artists => artists.category === this.$route.params.category)
		}
	},
	methods: {
		sortArtistsByNameAscending() {
			this.artists.sort((first, second) => first.name <= second.name ? -1 : 1);
		},
		sortArtistsByNameDescending() {
			this.artists.sort((first, second) => first.name >= second.name ? -1 : 1);
		}
	}
}
</script>