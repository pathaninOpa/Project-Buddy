# if never have venv use: python -m venv venv
install-ai-%:
	.\\venv\\Scripts\\activate && pip install "$(subst install-ai-,,$@)"
	.\\venv\\Scripts\\activate && pip freeze > requirements.txt

checkdp-ai:
	.\\venv\\Scripts\\activate && pip list
build-ai:
	docker compose up --build ai
	docker image prune -f

run-ai:
	docker compose up ai

stop-ai:
	docker compose down ai

