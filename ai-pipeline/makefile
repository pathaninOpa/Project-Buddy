# if never have venv use: python -m venv venv then ./venv/Scripts/activate

freeze:
	pip freeze > requirements.txt

build-ai:
	docker compose up --build ai
	docker image prune -f

run-ai:
	docker compose up ai

stop-ai:
	docker compose down ai

