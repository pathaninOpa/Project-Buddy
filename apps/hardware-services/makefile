# if never have venv use: python -m venv venv

freeze:
	pip freeze > requirements.txt

activate:
	.\\venv\\Scripts\\activate

build-hardware:
	docker compose up --build hardware
	docker image prune -f

run-hardware:
	docker compose up hardware

stop-hardware:
	docker compose down hardware

