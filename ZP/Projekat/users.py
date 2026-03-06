from dataclasses import dataclass
from cryptography.hazmat.primitives.asymmetric import rsa
from PyQt6.QtWidgets import QInputDialog, QLineEdit
from cryptography.hazmat.primitives.hashes import Hash, SHA1, MD5
from cryptography.hazmat.primitives.ciphers import Cipher
from cryptography.hazmat.primitives.ciphers.algorithms import CAST5
from cryptography.hazmat.primitives.ciphers.modes import CFB
from cryptography.hazmat.primitives import serialization

@dataclass
class UserInfo:
    name: str
    email: str


@dataclass
class PrivateKey:
    info: UserInfo
    encrypted_private_key: bytes
    public_key: rsa.RSAPublicKey
    
    @property
    def id(self):
        return self.public_key.public_numbers().n % (2 ** 64)
    
    @property	
    def private_key(self) -> rsa.RSAPrivateKey | None:
        password, ok = QInputDialog.getText(
            None,
            "Access private key",
            f"Password for {self.info.email}",
            QLineEdit.EchoMode.Password
        )
        if not ok:
            return None
        
        return serialization.load_pem_private_key(
            self.encrypted_private_key,
            password.encode('utf-8')
        )


@dataclass
class PublicKey:
    info: UserInfo
    key: rsa.RSAPublicKey

    @property
    def id(self):
        return self.key.public_numbers().n % (2 ** 64)
