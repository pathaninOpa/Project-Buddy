install-ai-%:
	python -m venv venv
	.\\venv\\Scripts\\activate && pip install "$(subst install-ai-,,$@)"
	.\\venv\\Scripts\\activate && pip freeze > requirements.txt

build-ai:
	docker compose up --build ai
	docker image prune -f

run-ai:
	docker compose up ai

stop-ai:
	docker compose down ai

