<template>
	<nav class="navbar navbar-expand-lg bg-body-tertiary fixed-top">
		<div class="container-fluid">
			<a class="navbar-brand" href="#">
				<img src="icon.png" height="30" class="d-inline-block align-text-top">
				{{ $t("title") }}
			</a>

			<div class="collapse navbar-collapse">
				<ul class="navbar-nav me-auto mb-2 mb-lg-0">

				</ul>

				<form class="d-flex">
					<input class="form-control me-2" type="search" placeholder="Претрага">
					<button class="btn btn-outline-success" type="submit">Претражи</button>
				</form>
			</div>
		</div>
	</nav>
</template>

<script>
export default {
	props: {

	}
}
</script>

<style scoped>

</style>
