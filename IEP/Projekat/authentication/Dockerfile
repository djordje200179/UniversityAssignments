FROM python:3

RUN mkdir /code
WORKDIR /code

COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY *.py .

ENTRYPOINT ["python", "app.py"]