from typing import override, Callable
from PyQt6 import uic
from PyQt6.QtWidgets import QDialog, QLineEdit
from users import UserInfo


class KeyInfoForm(uic.loadUiType("ui/key_info_form.ui")[0], QDialog):
    on_info_input: Callable[[UserInfo], None]

    nameInput: QLineEdit
    emailInput: QLineEdit

    def __init__(self, on_info_input: Callable[[UserInfo], None]):
        super().__init__()
        super(QDialog).__init__()

        self.on_info_input = on_info_input

        self.setupUi(self)

    @override
    def accept(self):
        name = self.nameInput.text()
        email = self.emailInput.text()

        self.on_info_input(UserInfo(name, email))

        self.close()
