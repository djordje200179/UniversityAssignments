import base64
import io
from dataclasses import dataclass


@dataclass
class Signature:
    sender_key_id: int
    digest: bytes  # 128/256 bytes


@dataclass
class Encryption:
    recipient_key_id: int
    session_key: bytes # 128/256 bytes
    algorithm: str


@dataclass
class Message:
    text: bytes

    signature: Signature | None
    encryption: Encryption | None

    compressed: bool
    encoded: bool

    def write(self, file_path: str):
        header = ""
        header += "1" if self.encoded else "0"
        header += "1" if self.signature is not None else "0"
        header += "1" if self.compressed else "0"
        if self.encryption is not None:
            match self.encryption.algorithm:
                case "TripleDES":
                    header += "1"
                case "AES128":
                    header += "2"
        else:
            header += "0"

        data = bytearray()
        data += len(self.text).to_bytes(8, "big")
        data += self.text

        if self.signature is not None:
            data += self.signature.sender_key_id.to_bytes(8, "big")
            data += len(self.signature.digest).to_bytes(8, "big")
            data += self.signature.digest
        
        if self.encryption is not None:
            data += self.encryption.recipient_key_id.to_bytes(8, "big")
            data += len(self.encryption.session_key).to_bytes(8, "big")
            data += self.encryption.session_key

        if self.encoded:
            data = base64.b64encode(data)

        with open(file_path, "wb") as file:
            file.write(header.encode("ascii"))
            file.write(b"\n")
            file.write(data)
            

    @staticmethod
    def read(file_path: str):
        with open(file_path, "rb") as file:
            header, data = file.read().split(b"\n", 1)
        
        header = header.decode("ascii")

        encoded = header[0] == "1"
        has_signature = header[1] == "1"
        compressed = header[2] == "1"
        encryption_type = int(header[3])

        if encoded:
            data = base64.b64decode(data)

        with io.BytesIO(data) as file:
            text_length = int.from_bytes(file.read(8), "big")
            text = file.read(text_length)

            signature = None
            if has_signature:
                sender_key_id = int.from_bytes(file.read(8), "big")
                digest_length = int.from_bytes(file.read(8), "big")
                digest = file.read(digest_length)
                signature = Signature(sender_key_id, digest)

            encryption = None
            if encryption_type != 0:
                recipient_key_id = int.from_bytes(file.read(8), "big")
                session_key_length = int.from_bytes(file.read(8), "big")
                session_key = file.read(session_key_length)
                match encryption_type:
                    case 1:
                        algorithm = "TripleDES"
                    case 2:
                        algorithm = "AES128"
                encryption = Encryption(recipient_key_id, session_key, algorithm)

        return Message(text, signature, encryption, compressed, encoded)
