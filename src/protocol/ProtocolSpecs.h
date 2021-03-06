#ifndef OPENMITTSU_PROTOCOL_PROTOCOLSPECS_H_
#define OPENMITTSU_PROTOCOL_PROTOCOLSPECS_H_

#include <QByteArray>
#include <cstdint>

#include <sodium.h>

#define PROTO_FINGERPRINT_LENGTH_BYTES (16)
#define PROTO_KEY_LENGTH_BYTES (32)
#define PROTO_IDENTITY_LENGTH_BYTES (8)
#define PROTO_NONCE_LENGTH_BYTES (16)
#define PROTO_NONCE_PREFIX_LENGTH_BYTES (16)
#define PROTO_AUTHENTICATION_UNENCRYPTED_LENGTH_BYTES (128)
#define PROTO_AUTHENTICATION_VERSION_BYTES (32)
#define PROTO_AUTHENTICATION_RANDOMNONCE_BYTES (24)
#define PROTO_AUTHENTICATION_REPLY_LENGTH_BYTES (32)

#define PROTO_DATA_HEADER_SIZE_LENGTH_BYTES (2)
#define PROTO_DATA_HEADER_TYPE_LENGTH_BYTES (4)
#define PROTO_ACKNOWLEDGMENT_LENGTH_BYTES (20)

#define PROTO_MESSAGE_TIMESTAMP_LENGTH_BYTES (4)
#define PROTO_MESSAGE_FLAGS_LENGTH_BYTES (1)
#define PROTO_MESSAGE_RESERVED_AFTER_FLAGS_LENGTH_BYTES (3)
#define PROTO_MESSAGE_MESSAGEID_LENGTH_BYTES (8)
#define PROTO_MESSAGE_PUSH_FROM_LENGTH_BYTES (32)
#define PROTO_MESSAGE_HEADER_FULL_LENGTH_BYTES ((PROTO_IDENTITY_LENGTH_BYTES) + (PROTO_IDENTITY_LENGTH_BYTES) + (PROTO_MESSAGE_MESSAGEID_LENGTH_BYTES) + (PROTO_MESSAGE_TIMESTAMP_LENGTH_BYTES) + (PROTO_MESSAGE_FLAGS_LENGTH_BYTES) + (PROTO_MESSAGE_RESERVED_AFTER_FLAGS_LENGTH_BYTES) + (PROTO_MESSAGE_PUSH_FROM_LENGTH_BYTES))

#define PROTO_MESSAGE_PADDING_MAX_LENGTH_BYTES (255)

#define PROTO_MESSAGE_NONCE_LENGTH_BYTES (crypto_box_NONCEBYTES)

#define PROTO_MESSAGE_PADDING_MAX_LENGTH_BYTES (255)
#define PROTO_MESSAGE_CONTENT_TYPE_LENGTH_BYTES (1)

#define PROTO_FILE_ENCRYPTION_KEY_LENGTH_BYTES (crypto_secretbox_KEYBYTES)

#define PROTO_PACKET_MAX_LENGTH_BYTES (4096)
#define PROTO_MESSAGE_CONTENT_PAYLOAD_MAX_LENGTH_BYTES ((PROTO_PACKET_MAX_LENGTH_BYTES) - (PROTO_DATA_HEADER_TYPE_LENGTH_BYTES) - (crypto_box_MACBYTES) - (crypto_box_MACBYTES) - (PROTO_MESSAGE_CONTENT_TYPE_LENGTH_BYTES) - (PROTO_MESSAGE_NONCE_LENGTH_BYTES) - (PROTO_MESSAGE_HEADER_FULL_LENGTH_BYTES) - (PROTO_MESSAGE_PADDING_MAX_LENGTH_BYTES))

		// Packet Types
#define PROTO_PACKET_SIGNATURE_KEEPALIVE_REQUEST ((char)0)					// 0x00
#define PROTO_PACKET_SIGNATURE_SENDING_MSG ((char)1)						// 0x01
#define PROTO_PACKET_SIGNATURE_DELIVERING_MSG ((char)2)						// 0x02
#define PROTO_PACKET_SIGNATURE_KEEPALIVE_ANSWER ((char)-128)				// 0x80
#define PROTO_PACKET_SIGNATURE_SERVER_ACK ((char)-127)						// 0x81
#define PROTO_PACKET_SIGNATURE_CLIENT_ACK ((char)-126)						// 0x82
#define PROTO_PACKET_SIGNATURE_CONNECTION_ESTABLISHED ((char)-48)			// 0xD0
#define PROTO_PACKET_SIGNATURE_CONNECTION_DUPLICATE ((char)-32)				// 0xE0

// Payload Signature Bytes
#define PROTO_MESSAGE_SIGNATURE_CONTACT_TEXT ((char)1)						// 0x01
#define PROTO_MESSAGE_SIGNATURE_CONTACT_PICTURE ((char)2)					// 0x02
#define PROTO_MESSAGE_SIGNATURE_CONTACT_LOCATION ((char)16)					// 0x10
#define PROTO_MESSAGE_SIGNATURE_CONTACT_VIDEO ((char)19)					// 0x13
#define PROTO_MESSAGE_SIGNATURE_GROUP_TEXT ((char)65)						// 0x41
#define PROTO_MESSAGE_SIGNATURE_GROUP_LOCATION ((char)66)					// 0x42
#define PROTO_MESSAGE_SIGNATURE_GROUP_PICTURE ((char)67)					// 0x43
#define PROTO_MESSAGE_SIGNATURE_GROUP_VIDEO ((char)68)						// 0x44
#define PROTO_MESSAGE_SIGNATURE_GROUP_FILE ((char)70)						// 0x46
#define PROTO_MESSAGE_SIGNATURE_GROUP_CREATION ((char)74)					// 0x4A
#define PROTO_MESSAGE_SIGNATURE_GROUP_TITLE ((char)75)						// 0x4B
#define PROTO_MESSAGE_SIGNATURE_GROUP_LEAVE ((char)76)						// 0x4C
#define PROTO_MESSAGE_SIGNATURE_GROUP_PHOTO ((char)80)						// 0x50
#define PROTO_MESSAGE_SIGNATURE_GROUP_SYNC ((char)81)						// 0x51
#define PROTO_MESSAGE_SIGNATURE_RECEIPT ((char)-128)						// 0x80
#define PROTO_MESSAGE_SIGNATURE_TYPINGNOTIFICATION ((char)-112)				// 0x90

// Receipt Types
#define PROTO_RECEIPT_TYPE_RECEIVED ((char)1)								// 0x01
#define PROTO_RECEIPT_TYPE_SEEN ((char)2)									// 0x02
#define PROTO_RECEIPT_TYPE_AGREE ((char)3)									// 0x03
#define PROTO_RECEIPT_TYPE_DISAGREE ((char)4)								// 0x04

// Typing Notification Types
#define PROTO_TYPINGNOTIFICATION_TYPE_STOPPED ((char)0)						// 0x00
#define PROTO_TYPINGNOTIFICATION_TYPE_TYPING ((char)1)						// 0x01

#define PROTO_GROUP_GROUPID_LENGTH_BYTES (8)

#define PROTO_NONCE_SERVER_LENGTH_BYTES (16)
#define PROTO_SERVERHELLO_CYPHERTEXT_LENGTH_BYTES (64)
#define PROTO_SERVERHELLO_LENGTH_BYTES (PROTO_NONCE_SERVER_LENGTH_BYTES + PROTO_SERVERHELLO_CYPHERTEXT_LENGTH_BYTES)

#define PROTO_KEEPALIVE_REQUEST_MAX_LENGTH_BYTES (128)

#define PROTO_IMAGESERVER_ID_LENGTH_BYTES (16)
#define PROTO_IMAGESERVER_KEY_LENGTH_BYTES (24)
#define PROTO_IMAGESERVER_SIZE_LENGTH_BYTES (4)

#define BACKUP_LENGTH_BYTES (256)
#define BACKUP_SALT_BYTES (8)
#define BACKUP_IDENTITY_BYTES (PROTO_IDENTITY_LENGTH_BYTES)
#define BACKUP_HASH_BYTES (2)
#define BACKUP_DECODED_BYTES (BACKUP_SALT_BYTES + BACKUP_IDENTITY_BYTES + PROTO_KEY_LENGTH_BYTES + BACKUP_HASH_BYTES)
#define BACKUP_ENCRYPTION_KEY_BYTES (32)
#define BACKUP_KEY_PBKDF_ITERATIONS (100000)

namespace openmittsu {
	namespace protocol {
		//
	}
}

#endif // OPENMITTSU_PROTOCOL_PROTOCOLSPECS_H_