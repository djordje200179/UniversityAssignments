import os
from typing import override

from PyQt6 import uic
from PyQt6.QtWidgets import QMainWindow, QTableWidget, QFileDialog, QInputDialog, QMessageBox, QTableWidgetItem, \
    QPushButton, QHBoxLayout, QWidget, QComboBox, QTextBrowser, QCheckBox, QLineEdit
from cryptography.exceptions import InvalidSignature
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric.padding import OAEP, MGF1, PSS
from cryptography.hazmat.primitives.ciphers import Cipher
from cryptography.hazmat.primitives.ciphers.algorithms import AES, TripleDES, CAST5
from cryptography.hazmat.primitives.ciphers.modes import CFB, ECB
from cryptography.hazmat.primitives.hashes import SHA1, SHA256, MD5
from cryptography.hazmat.primitives.asymmetric import rsa, utils
from zlib import compress, decompress

from message import Message, Signature, Encryption
from users import PrivateKey, PublicKey, UserInfo
from ui.key_info_form import KeyInfoForm


class MainWindow(uic.loadUiType("ui/main_window.ui")[0], QMainWindow):
    key_info_form: KeyInfoForm

    private_ring: list[PrivateKey]
    public_ring: list[PublicKey]

    privateRingTable: QTableWidget
    publicRingTable: QTableWidget

    senderInput: QComboBox
    recipientInput: QComboBox
    sendMessageInput: QTextBrowser
    signMessageInput: QCheckBox
    compressMessageInput: QCheckBox
    encodeMessageInput: QCheckBox
    encryptionInput: QComboBox

    def __init__(self):
        super().__init__()
        super(QMainWindow).__init__()

        self.private_ring = []
        self.public_ring = []

        self.setupUi(self)

        self.privateRingTable.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        self.publicRingTable.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)

    @override
    def newKeyPressed(self):
        key_size, ok = QInputDialog.getItem(self, "New key", "Select key size", ["1024", "2048"], 0, False)
        if not ok:
            return
        key_size = int(key_size)

        private_key = rsa.generate_private_key(65537, key_size=key_size)

        password, ok = QInputDialog.getText(self, "New key", "Password", QLineEdit.EchoMode.Password)
        if not ok:
            return

        self.key_info_form = KeyInfoForm(lambda info: self.append_private_key(password, private_key, info))
        self.key_info_form.exec()

    @override
    def importKeyPressed(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Import key", "", "RSA keys (*.pem)")
        if not file_path:
            return

        with open(file_path, "rb") as f:
            file_content = f.read()
            file_lines = file_content.split(b"\n")

            for i, line in enumerate(file_lines):
                if line == b"-----BEGIN EMAIL ADDRESS-----":
                    email = file_lines[i + 1].decode('utf-8')
                    break

            for i, line in enumerate(file_lines):
                if line == b"-----BEGIN NAME-----":
                    name = file_lines[i + 1].decode('utf-8')
                    break

            match file_lines[0]:
                case b"-----BEGIN ENCRYPTED PRIVATE KEY-----":
                    password, ok = QInputDialog.getText(self, "Import key", "Password", QLineEdit.EchoMode.Password)
                    if not ok:
                        return

                    try:
                        key = serialization.load_pem_private_key(file_content, password.encode('utf-8'))
                    except ValueError:
                        QMessageBox.critical(self, "Import key", "Invalid password")
                        return

                    self.append_private_key(password, key, UserInfo(name, email))
                case b"-----BEGIN PUBLIC KEY-----":
                    key = serialization.load_pem_public_key(file_content)
                    self.append_public_key(key, UserInfo(name, email))
                case _:
                    raise ValueError("Invalid key format")


    @override
    def readMessagePressed(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Read message", "", "Message (*.msg)")
        if not file_path:
            return

        message = Message.read(file_path)
        message_text = message.text

        if message.encryption is not None:
            recipient_key = next((key for key in self.private_ring if key.id == message.encryption.recipient_key_id), None)
            if not recipient_key:
                QMessageBox.critical(self, "Message decryption", "Recipient key not found")
                return

            try:
                private_key = recipient_key.private_key
                if private_key is None:
                    QMessageBox.critical(self, "Message decryption", "Password not provided.")
                    return
            except ValueError:
                QMessageBox.critical(self, "Message decryption", "Invalid password")
                return

            session_key = private_key.decrypt(
                message.encryption.session_key,
                OAEP(MGF1(SHA1()), SHA1(), None)
            )

            match message.encryption.algorithm:
                case "TripleDES":
                    cipher = TripleDES(session_key)
                case "AES128":
                    cipher = AES(session_key)

            decryptor = Cipher(cipher, CFB(b"\x00" * 8)).decryptor()

            message_text = decryptor.update(message_text) + decryptor.finalize()

        if message.compressed:
            message_text = decompress(message_text)

        if message.signature is not None:
            sender_key = next((key for key in self.public_ring if key.id == message.signature.sender_key_id), None)
            if sender_key:
                try:
                    sender_key.key.verify(
                        message.signature.digest,
                        message_text,
                        PSS(MGF1(SHA1()), 20),
                        SHA1()
                    )
                except InvalidSignature:
                    QMessageBox.critical(self, "Signature verification", "Invalid signature")
                    return

                QMessageBox.information(
                    self, "Signature verification",
                    f"Message was sent by {sender_key.info.name} ({sender_key.info.email})"
                )
            else:
                QMessageBox.warning(self, "Signature verification", "Sender key not found")

        file_path, _ = QFileDialog.getSaveFileName(self, "Save message text", "", "Text (*.txt)")
        
        if not file_path:
            return

        with open(file_path, "w") as f:
            f.write(message_text.decode('utf-8'))

    @override
    def sendMessagePressed(self):
        message_text = self.sendMessageInput.toPlainText().encode('utf-8')

        if self.signMessageInput.isChecked():
            sender_key: PrivateKey = self.senderInput.currentData()

            try:
                sender_private_key: rsa.RSAPrivateKey = sender_key.private_key
                if sender_private_key is None:
                    QMessageBox.critical(self, "Signature verification", "Password not provided.")
                    return
                else:
                    signature = Signature(
                        sender_key.id,
                        sender_private_key.sign(message_text, PSS(MGF1(SHA1()), 20), SHA1())
                )
            except ValueError:
                QMessageBox.critical(self, "Signature verification", "Invalid password")
                return
        else:
            signature = None

        if self.compressMessageInput.isChecked():
            message_text = compress(message_text)
            
        if (algorithm := self.encryptionInput.currentText()) != "":
            session_key = os.urandom(16)
            match algorithm:
                case "TripleDES":
                    cipher = TripleDES(session_key)
                case "AES128":
                    cipher = AES(session_key)

            encryptor = Cipher(cipher, CFB(b"\x00" * 8)).encryptor()

            message_text = encryptor.update(message_text) + encryptor.finalize()

            recipient_key: PublicKey = self.recipientInput.currentData()
            encryption = Encryption(
                recipient_key.id,
                recipient_key.key.encrypt(session_key, OAEP(MGF1(SHA1()), SHA1(), None)),
                algorithm
            )
        else:
            encryption = None

        message = Message(
            message_text,
            signature,
            encryption,
            self.compressMessageInput.isChecked(),
            self.encodeMessageInput.isChecked()
        )

        file_path, _ = QFileDialog.getSaveFileName(self, "Save message", "", "Message (*.msg)")
        if not file_path:
            return

        message.write(file_path)

    def delete_entry(table: QTableWidget, row_index: int, _something):
        table.removeRow(row_index)

    @override
    def encryptionAlgorithmChanged(self, index: int):
        self.recipientInput.setEnabled(index != 0)

    def append_public_key(self, key: rsa.RSAPublicKey, info: UserInfo):
        
        public_key = PublicKey(info, key)
        self.public_ring.append(public_key)

        row_index = self.publicRingTable.rowCount()

        def delete_key():
            self.publicRingTable.removeRow(row_index)

        print(type(row_index))
        print(type(self.publicRingTable))
        self.publicRingTable.insertRow(row_index)
        self.publicRingTable.setItem(row_index, 0, QTableWidgetItem(hex(public_key.id)))
        self.publicRingTable.setItem(row_index, 1, QTableWidgetItem(public_key.info.name))
        self.publicRingTable.setItem(row_index, 2, QTableWidgetItem(public_key.info.email))
        delete_button = QPushButton('Delete')
        delete_button.clicked.connect(delete_key)
        self.publicRingTable.setCellWidget(row_index, 3, delete_button )

        self.recipientInput.addItem(
            f"{public_key.info.name} ({public_key.info.email}) - {hex(public_key.id)}",
            public_key
        )

    def append_private_key(self, password: str, key: rsa.RSAPrivateKey, info: UserInfo):
        encrypted_private_key = key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.PKCS8,
            encryption_algorithm=serialization.BestAvailableEncryption(password.encode('utf-8'))
        )

        public_key = key.public_key()

        private_key = PrivateKey(info, encrypted_private_key, public_key)
        self.private_ring.append(private_key)

        row_index = self.privateRingTable.rowCount()
        self.privateRingTable.insertRow(row_index)
        self.privateRingTable.setItem(row_index, 0, QTableWidgetItem(hex(private_key.id)))
        self.privateRingTable.setItem(row_index, 1, QTableWidgetItem(private_key.info.name))
        self.privateRingTable.setItem(row_index, 2, QTableWidgetItem(private_key.info.email))

        self.senderInput.addItem(
            f"{private_key.info.name} ({private_key.info.email}) - {hex(private_key.id)}",
            private_key
        )

        def export_key():
            file_path, _ = QFileDialog.getSaveFileName(self, "Export key", "", "RSA key (*.pem)")
            if not file_path:
                return

            with open(file_path, "wb") as f:
                f.write(encrypted_private_key)

                f.write(b"-----BEGIN EMAIL ADDRESS-----\n")
                f.write(private_key.info.email.encode('utf-8'))
                f.write(b"\n-----END EMAIL ADDRESS-----\n")
                f.write(b"-----BEGIN NAME-----\n")
                f.write(private_key.info.name.encode('utf-8'))
                f.write(b"\n-----END NAME-----\n")

        def export_public_key():
            pem = private_key.public_key.public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo,
            )

            file_path, _ = QFileDialog.getSaveFileName(self, "Export key", "", "RSA public key (*.pem)")
            if not file_path:
                return

            with open(file_path, "wb") as f:
                f.write(pem)

                f.write(b"-----BEGIN EMAIL ADDRESS-----\n")
                f.write(private_key.info.email.encode('utf-8'))
                f.write(b"\n-----END EMAIL ADDRESS-----\n")
                f.write(b"-----BEGIN NAME-----\n")
                f.write(private_key.info.name.encode('utf-8'))
                f.write(b"\n-----END NAME-----\n")

        def delete_key():
            self.privateRingTable.removeRow(row_index)

        export_key_button = QPushButton("Full")
        export_key_button.clicked.connect(export_key)
        export_public_key_button = QPushButton("Public")
        export_public_key_button.clicked.connect(export_public_key)
        delete_key_button = QPushButton("Delete")
        delete_key_button.clicked.connect(delete_key)

        self.privateRingTable.setCellWidget(row_index, 3, export_key_button)
        self.privateRingTable.setCellWidget(row_index, 4, export_public_key_button)
        self.privateRingTable.setCellWidget(row_index, 5, delete_key_button)

        # export_buttons_layout = QHBoxLayout()
        # export_buttons_layout.addWidget(export_key_button)
        # export_buttons_layout.addWidget(export_public_key_button)
        # export_buttons_layout.addWidget(delete_key_button)

        # export_buttons = QWidget()
        # export_buttons.setLayout(export_buttons_layout)

        # self.privateRingTable.setCellWidget(row_index, 3, export_buttons)
