#include "gtest/gtest.h"

#include <QString>
#include <QSet>
#include <QList>
#include <QVariant>

#include "exceptions/InternalErrorException.h"
#include "crypto/Crc32.h"

#include "DatabaseTestFramework.h"

TEST_F(DatabaseTestFramework, createNew) {
	ASSERT_EQ(0, db->getGroupCount());
	ASSERT_EQ(1, db->getContactCount());

	openmittsu::backup::IdentityBackup const backupData = db->getBackup();
	ASSERT_EQ(selfContactId, backupData.getClientContactId());
	ASSERT_EQ(selfKeyPair, backupData.getClientLongTermKeyPair());

	openmittsu::protocol::GroupId nonExistantGroupId(selfContactId, 0);
	ASSERT_FALSE(db->hasGroup(nonExistantGroupId));

	openmittsu::protocol::ContactId nonExistantContactId(QStringLiteral("BBBBBBBB"));
	ASSERT_FALSE(db->hasContact(nonExistantContactId));

	openmittsu::protocol::ContactId contactIdC(QStringLiteral("CCCCCCCC"));
	openmittsu::crypto::KeyPair contactIdCKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_FALSE(db->hasContact(contactIdC));
	ASSERT_NO_THROW(db->storeNewContact(contactIdC, contactIdCKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdC));
	ASSERT_EQ(2, db->getContactCount());
	ASSERT_NO_THROW(db->storeNewContact(contactIdC, contactIdCKeyPair));
	ASSERT_EQ(2, db->getContactCount());
}

TEST_F(DatabaseTestFramework, contacts) {
	/*
	PublicKey getContactPublicKey(ContactId const& identity) const;
	QString getContactNickname(ContactId const& identity) const;
	AccountStatus getContactStatus(ContactId const& identity) const;
	ContactIdVerificationStatus getContactVerficationStatus(ContactId const& identity) const;
	FeatureLevel getContactFeatureLevel(ContactId const& identity) const;

	QList<ContactId> getKnownContacts() const;
	QHash<ContactId, PublicKey> getKnownContactsWithPublicKeys() const;

	void setContactNickname(ContactId const& identity, QString const& nickname);	
	*/
	
	ASSERT_EQ(0, db->getGroupCount());
	ASSERT_EQ(1, db->getContactCount());

	openmittsu::protocol::ContactId nonExistantContactId(QStringLiteral("BBBBBBBB"));
	ASSERT_FALSE(db->hasContact(nonExistantContactId));

	openmittsu::protocol::ContactId contactIdC(QStringLiteral("CCCCCCCC"));
	openmittsu::crypto::KeyPair contactIdCKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_FALSE(db->hasContact(contactIdC));
	ASSERT_NO_THROW(db->storeNewContact(contactIdC, contactIdCKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdC));
	ASSERT_EQ(2, db->getContactCount());
	ASSERT_NO_THROW(db->storeNewContact(contactIdC, contactIdCKeyPair));
	ASSERT_EQ(2, db->getContactCount());

	openmittsu::crypto::PublicKey const contactIdCPublicKey = contactIdCKeyPair;
	openmittsu::crypto::PublicKey const publicKeyIdCFromDatabase = db->getContactPublicKey(contactIdC);

	ASSERT_EQ(contactIdCPublicKey, publicKeyIdCFromDatabase);
	ASSERT_EQ(QString(""), db->getContactNickname(contactIdC));
	ASSERT_EQ(openmittsu::protocol::ContactStatus::KNOWN, db->getContactStatus(contactIdC));
	ASSERT_EQ(openmittsu::protocol::AccountStatus::STATUS_UNKNOWN, db->getContactAccountStatus(contactIdC));
	ASSERT_EQ(openmittsu::protocol::ContactIdVerificationStatus::VERIFICATION_STATUS_UNVERIFIED, db->getContactVerficationStatus(contactIdC));
	ASSERT_EQ(openmittsu::protocol::FeatureLevel::LEVEL_UNKNOW , db->getContactFeatureLevel(contactIdC));

	QSet<openmittsu::protocol::ContactId> const knownContacts = db->getKnownContacts();
	ASSERT_EQ(2, knownContacts.size());
	ASSERT_TRUE(knownContacts.contains(contactIdC));

	QHash<openmittsu::protocol::ContactId, openmittsu::crypto::PublicKey> const knownContactsWithKeys = db->getKnownContactsWithPublicKeys();
	ASSERT_EQ(2, knownContactsWithKeys.size());
	ASSERT_TRUE(knownContactsWithKeys.contains(contactIdC));
	ASSERT_EQ(contactIdCPublicKey, *knownContactsWithKeys.constFind(contactIdC));

	openmittsu::protocol::ContactId contactIdD(QStringLiteral("DDDDDDDD"));
	openmittsu::crypto::KeyPair contactIdDKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_FALSE(db->hasContact(contactIdD));
	ASSERT_NO_THROW(db->storeNewContact(contactIdD, contactIdDKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdD));
	ASSERT_EQ(3, db->getContactCount());

	QString const newNickname(QStringLiteral("testNickName"));
	ASSERT_THROW(db->setContactNickname(nonExistantContactId, newNickname), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(db->setContactNickname(contactIdC, newNickname));
	ASSERT_EQ(newNickname, db->getContactNickname(contactIdC));

	openmittsu::protocol::AccountStatus const newContactIdStatus = openmittsu::protocol::AccountStatus::STATUS_ACTIVE;
	ASSERT_THROW(db->setContactAccountStatus(nonExistantContactId, newContactIdStatus), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(db->setContactAccountStatus(contactIdC, newContactIdStatus));
	ASSERT_EQ(newContactIdStatus, db->getContactAccountStatus(contactIdC));

	openmittsu::protocol::ContactIdVerificationStatus const newContactIdVerificationStatus = openmittsu::protocol::ContactIdVerificationStatus::VERIFICATION_STATUS_FULLY_VERIFIED;
	ASSERT_THROW(db->setContactVerficationStatus(nonExistantContactId, newContactIdVerificationStatus), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(db->setContactVerficationStatus(contactIdC, newContactIdVerificationStatus));
	ASSERT_EQ(newContactIdVerificationStatus, db->getContactVerficationStatus(contactIdC));

	openmittsu::protocol::FeatureLevel const newContactFeatureLevel = openmittsu::protocol::FeatureLevel::LEVEL_3;
	ASSERT_THROW(db->setContactFeatureLevel(nonExistantContactId, newContactFeatureLevel), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(db->setContactFeatureLevel(contactIdC, newContactFeatureLevel));
	ASSERT_EQ(newContactFeatureLevel, db->getContactFeatureLevel(contactIdC));

	openmittsu::crypto::PublicKey const contactIdDPublicKey = contactIdDKeyPair;
	openmittsu::crypto::PublicKey const publicKeyIdDFromDatabase = db->getContactPublicKey(contactIdD);
	ASSERT_EQ(contactIdDPublicKey, publicKeyIdDFromDatabase);
	ASSERT_EQ(QString(""), db->getContactNickname(contactIdD));
	ASSERT_EQ(openmittsu::protocol::ContactStatus::KNOWN, db->getContactStatus(contactIdD));
	ASSERT_EQ(openmittsu::protocol::AccountStatus::STATUS_UNKNOWN, db->getContactAccountStatus(contactIdD));
	ASSERT_EQ(openmittsu::protocol::ContactIdVerificationStatus::VERIFICATION_STATUS_UNVERIFIED, db->getContactVerficationStatus(contactIdD));
	ASSERT_EQ(openmittsu::protocol::FeatureLevel::LEVEL_UNKNOW, db->getContactFeatureLevel(contactIdD));
}

TEST_F(DatabaseTestFramework, contactMessages) {
	openmittsu::protocol::ContactId contactIdB(QStringLiteral("BBBBBBBB"));
	openmittsu::crypto::KeyPair contactIdBKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_NO_THROW(db->storeNewContact(contactIdB, contactIdBKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdB));
	
	openmittsu::protocol::ContactId contactIdC(QStringLiteral("CCCCCCCC"));
	openmittsu::crypto::KeyPair contactIdCKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_NO_THROW(db->storeNewContact(contactIdC, contactIdCKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdC));

	openmittsu::protocol::ContactId contactIdD(QStringLiteral("DDDDDDDD"));
	openmittsu::crypto::KeyPair contactIdDKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_NO_THROW(db->storeNewContact(contactIdD, contactIdDKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdD));

	openmittsu::protocol::MessageId messageA(0);
	openmittsu::protocol::MessageId messageB(0);
	openmittsu::protocol::MessageId messageC(0);
	openmittsu::protocol::MessageId messageD(0);
	openmittsu::protocol::MessageId messageE(0);

	ASSERT_NO_THROW(messageA = db->storeSentContactMessageText(contactIdB, openmittsu::protocol::MessageTime::fromDatabase(12345678), true, QStringLiteral("TestMessageA")));
	this->addMessageId(messageA);

	messageB = this->getFreeMessageId();
	ASSERT_NO_THROW(db->storeReceivedContactMessageText(contactIdB, messageB, openmittsu::protocol::MessageTime::fromDatabase(123), openmittsu::protocol::MessageTime::fromDatabase(12345678), QStringLiteral("TestMessageB")));
	
	ASSERT_NO_THROW(messageC = db->storeSentContactMessageText(contactIdB, openmittsu::protocol::MessageTime::fromDatabase(23456789), true, QStringLiteral("TestMessageC")));
	this->addMessageId(messageC);

	openmittsu::utility::Location const location(1.2, 3.4, 5.6, QStringLiteral("Example Street 123, Somecity, Country"), QStringLiteral("Description"));
	ASSERT_NO_THROW(messageD = db->storeSentContactMessageLocation(contactIdD, openmittsu::protocol::MessageTime::fromDatabase(12345678), true, location));
	this->addMessageId(messageD);

	QByteArray const imageData = QStringLiteral("SomeTestDataThatRepresentsTheImage").toUtf8();
	ASSERT_NO_THROW(messageE = db->storeSentContactMessageImage(contactIdD, openmittsu::protocol::MessageTime::fromDatabase(23456789), true, imageData, QStringLiteral("An image Caption")));
	this->addMessageId(messageE);

	openmittsu::protocol::ContactId nonExistantContactId(QStringLiteral("XXXXXXXX"));
	ASSERT_THROW(db->getMessageCursor(nonExistantContactId), openmittsu::exceptions::InternalErrorException);

	openmittsu::database::DatabaseContactMessageCursor cursorB = db->getMessageCursor(contactIdB);
	openmittsu::database::DatabaseContactMessageCursor cursorC = db->getMessageCursor(contactIdC);
	openmittsu::database::DatabaseContactMessageCursor cursorD = db->getMessageCursor(contactIdD);
	ASSERT_FALSE(cursorB.isValid());
	ASSERT_FALSE(cursorC.isValid());
	ASSERT_FALSE(cursorD.isValid());

	// Seek all messages
	ASSERT_TRUE(cursorB.seek(messageA));
	ASSERT_TRUE(cursorB.seek(messageB));
	ASSERT_TRUE(cursorB.seek(messageC));
	ASSERT_FALSE(cursorB.seek(messageD));
	ASSERT_FALSE(cursorB.seek(messageE));

	ASSERT_FALSE(cursorC.seek(messageA));
	ASSERT_FALSE(cursorC.seek(messageB));
	ASSERT_FALSE(cursorC.seek(messageC));
	ASSERT_FALSE(cursorC.seek(messageD));
	ASSERT_FALSE(cursorC.seek(messageE));

	ASSERT_FALSE(cursorD.seek(messageA));
	ASSERT_FALSE(cursorD.seek(messageB));
	ASSERT_FALSE(cursorD.seek(messageC));
	ASSERT_TRUE(cursorD.seek(messageD));
	ASSERT_TRUE(cursorD.seek(messageE));

	// check seek First/Last
	ASSERT_TRUE(cursorB.seekToFirst());
	ASSERT_FALSE(cursorC.seekToFirst());

	ASSERT_TRUE(cursorB.isValid());
	ASSERT_FALSE(cursorC.isValid());

	ASSERT_FALSE(cursorB.previous());
	ASSERT_FALSE(cursorC.previous());

	ASSERT_TRUE((cursorB.getMessageId() == messageA) || (cursorB.getMessageId() == messageB));
	bool const wasMessageA = cursorB.getMessageId() == messageA;
	if (wasMessageA) {
		std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> message = cursorB.getMessage();
		ASSERT_EQ(message->getContactId(), contactIdB);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageA"));
	} else {
		std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> message = cursorB.getMessage();
		ASSERT_EQ(message->getContactId(), contactIdB);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageB"));
	}

	ASSERT_TRUE(cursorB.next());
	ASSERT_FALSE(cursorC.next());

	if (wasMessageA) {
		std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> message = cursorB.getMessage();
		ASSERT_EQ(messageB, cursorB.getMessageId());
		ASSERT_EQ(message->getContactId(), contactIdB);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageB"));
	} else {
		std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> message = cursorB.getMessage();
		ASSERT_EQ(messageA, cursorB.getMessageId());
		ASSERT_EQ(message->getContactId(), contactIdB);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageA"));
	}

	ASSERT_TRUE(cursorB.next());
	ASSERT_EQ(messageC, cursorB.getMessageId());

	std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> message = cursorB.getMessage();
	ASSERT_EQ(message->getContactId(), contactIdB);
	ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageC"));

	ASSERT_FALSE(cursorB.next());
	ASSERT_FALSE(cursorC.next());

	// and back again
	ASSERT_TRUE(cursorB.seekToFirst());
	ASSERT_FALSE(cursorC.seekToFirst());
	ASSERT_TRUE(cursorB.seekToLast());
	ASSERT_FALSE(cursorC.seekToLast());

	ASSERT_TRUE(cursorB.isValid());
	ASSERT_FALSE(cursorC.isValid());
	ASSERT_EQ(messageC, cursorB.getMessageId());

	{
		ASSERT_TRUE(cursorD.seek(messageD));
		std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> locationMessage = cursorD.getMessage();
		ASSERT_EQ(locationMessage->getContactId(), contactIdD);
		ASSERT_EQ(locationMessage->getContentAsLocation(), location);

		ASSERT_TRUE(cursorD.next());
		std::shared_ptr<openmittsu::dataproviders::messages::ContactMessage> imageMessage = cursorD.getMessage();
		ASSERT_EQ(imageMessage->getContactId(), contactIdD);
		ASSERT_TRUE(imageMessage->getContentAsImage().isAvailable());
		ASSERT_EQ(imageMessage->getContentAsImage().getData(), imageData);
	}
}

TEST_F(DatabaseTestFramework, groups) {
	/*
		QSet<openmittsu::protocol::GroupId> getKnownGroups() const;
		QSet<openmittsu::protocol::ContactId> getGroupMembers(openmittsu::protocol::GroupId const& group) const;
		QString getGroupTitle(openmittsu::protocol::GroupId const& group) const;
		QString getGroupDescription(openmittsu::protocol::GroupId const& group) const;
		int getGroupCount() const;
	*/

	ASSERT_EQ(0, db->getGroupCount());
	ASSERT_EQ(1, db->getContactCount());

	openmittsu::protocol::ContactId nonExistantContactId(QStringLiteral("BBBBBBBB"));
	openmittsu::protocol::ContactId contactIdC(QStringLiteral("CCCCCCCC"));
	openmittsu::crypto::KeyPair contactIdCKeyPair(openmittsu::crypto::KeyPair::randomKey());
	db->storeNewContact(contactIdC, contactIdCKeyPair);

	openmittsu::protocol::GroupId nonExistantGroupId(nonExistantContactId, 0);
	ASSERT_FALSE(db->hasGroup(nonExistantGroupId));
	ASSERT_THROW(db->storeNewGroup(nonExistantGroupId, { nonExistantContactId }, false), openmittsu::exceptions::InternalErrorException);
	ASSERT_FALSE(db->hasGroup(nonExistantGroupId));

	openmittsu::protocol::GroupId groupA(contactIdC, 1);
	ASSERT_FALSE(db->hasGroup(groupA));
	ASSERT_THROW(db->storeNewGroup(nonExistantGroupId, { nonExistantContactId }, false), openmittsu::exceptions::InternalErrorException);

	ASSERT_NO_THROW(db->storeNewGroup(groupA, { contactIdC, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupA));

	ASSERT_THROW(db->getGroupMembers(nonExistantGroupId, false), openmittsu::exceptions::InternalErrorException);
	QSet<openmittsu::protocol::ContactId> groupMembersA;
	ASSERT_NO_THROW(groupMembersA = db->getGroupMembers(groupA, false));
	ASSERT_EQ(2, groupMembersA.size());
	ASSERT_TRUE(groupMembersA.contains(contactIdC));

	QSet<openmittsu::protocol::GroupId> knownGroups;
	ASSERT_NO_THROW(knownGroups = db->getKnownGroups());
	ASSERT_EQ(1, knownGroups.size());
	ASSERT_TRUE(knownGroups.contains(groupA));

	// Add new contact + group
	openmittsu::protocol::ContactId contactIdD(QStringLiteral("DDDDDDDD"));
	openmittsu::crypto::KeyPair contactIdDKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_FALSE(db->hasContact(contactIdD));
	ASSERT_NO_THROW(db->storeNewContact(contactIdD, contactIdDKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdD));
	openmittsu::protocol::GroupId groupB(contactIdD, 1);
	ASSERT_NO_THROW(db->storeNewGroup(groupB, { contactIdD, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupB));

	ASSERT_NO_THROW(knownGroups = db->getKnownGroups());
	ASSERT_EQ(2, knownGroups.size());
	ASSERT_TRUE(knownGroups.contains(groupA));
	ASSERT_TRUE(knownGroups.contains(groupB));

	openmittsu::protocol::GroupId groupC(contactIdD, 2);
	ASSERT_NO_THROW(db->storeNewGroup(groupC, { contactIdD, contactIdC, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupC));

	openmittsu::protocol::GroupId groupD(contactIdD, 3);
	ASSERT_NO_THROW(db->storeNewGroup(groupD, { contactIdC, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupD));

	openmittsu::protocol::GroupId groupE(contactIdD, 4);
	ASSERT_NO_THROW(db->storeNewGroup(groupE, { contactIdC }, false));
	ASSERT_FALSE(db->hasGroup(groupE));
	ASSERT_TRUE(db->isDeleteted(groupE));


	//ASSERT_NO_THROW(db->onGroupSync(groupD, {}));
	openmittsu::protocol::MessageId const messageA = this->getFreeMessageId();
	db->storeReceivedGroupCreation(groupD, contactIdD, messageA, openmittsu::protocol::MessageTime::fromDatabase(123), openmittsu::protocol::MessageTime::fromDatabase(456), {});
	ASSERT_FALSE(db->hasGroup(groupD));
	ASSERT_TRUE(db->isDeleteted(groupD));

	ASSERT_NO_THROW(knownGroups = db->getKnownGroups());
	ASSERT_EQ(3, knownGroups.size());
	ASSERT_TRUE(knownGroups.contains(groupA));
	ASSERT_TRUE(knownGroups.contains(groupB));
	ASSERT_TRUE(knownGroups.contains(groupC));

	QSet<openmittsu::protocol::ContactId> groupCmembers;
	ASSERT_NO_THROW(groupCmembers = db->getGroupMembers(groupC, false));
	ASSERT_EQ(3, groupCmembers.size());
	ASSERT_TRUE(groupCmembers.contains(contactIdC));
	ASSERT_TRUE(groupCmembers.contains(contactIdD));

	ASSERT_THROW(db->getGroupTitle(nonExistantGroupId), openmittsu::exceptions::InternalErrorException);
	QString const emptyString(QStringLiteral(""));
	ASSERT_EQ(emptyString, db->getGroupTitle(groupA));
	ASSERT_EQ(emptyString, db->getGroupTitle(groupB));
	ASSERT_EQ(emptyString, db->getGroupTitle(groupC));

	QString const newGroupTitle(QStringLiteral("testTitle"));
	openmittsu::protocol::MessageId const messageB = this->getFreeMessageId();
	ASSERT_THROW(db->storeReceivedGroupSetTitle(nonExistantGroupId, nonExistantContactId, messageB, openmittsu::protocol::MessageTime::fromDatabase(123), openmittsu::protocol::MessageTime::fromDatabase(456), newGroupTitle), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(db->storeReceivedGroupSetTitle(groupC, contactIdD, messageB, openmittsu::protocol::MessageTime::fromDatabase(123), openmittsu::protocol::MessageTime::fromDatabase(456), newGroupTitle));

	ASSERT_EQ(emptyString, db->getGroupTitle(groupA));
	ASSERT_EQ(emptyString, db->getGroupTitle(groupB));
	ASSERT_EQ(newGroupTitle, db->getGroupTitle(groupC));

	// getKnownGroupsWithMembersAndTitles
	QHash<openmittsu::protocol::GroupId, std::pair<QSet<openmittsu::protocol::ContactId>, QString>> groupsWithMembersAndTitles;
	ASSERT_NO_THROW(groupsWithMembersAndTitles = db->getKnownGroupsWithMembersAndTitles());
	ASSERT_EQ(3, groupsWithMembersAndTitles.size());
	ASSERT_TRUE(groupsWithMembersAndTitles.contains(groupA));
	ASSERT_TRUE(groupsWithMembersAndTitles.contains(groupB));
	ASSERT_TRUE(groupsWithMembersAndTitles.contains(groupC));
	ASSERT_EQ(2, groupsWithMembersAndTitles.constFind(groupA)->first.size());
	ASSERT_EQ(2, groupsWithMembersAndTitles.constFind(groupB)->first.size());
	ASSERT_EQ(3, groupsWithMembersAndTitles.constFind(groupC)->first.size());
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupA)->first.contains(contactIdC));
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupA)->first.contains(selfContactId));
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupB)->first.contains(contactIdD));
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupB)->first.contains(selfContactId));
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupC)->first.contains(contactIdC));
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupC)->first.contains(contactIdD));
	ASSERT_TRUE(groupsWithMembersAndTitles.constFind(groupC)->first.contains(selfContactId));
	ASSERT_EQ(emptyString, groupsWithMembersAndTitles.constFind(groupA)->second);
	ASSERT_EQ(emptyString, groupsWithMembersAndTitles.constFind(groupB)->second);
	ASSERT_EQ(newGroupTitle, groupsWithMembersAndTitles.constFind(groupC)->second);

	QSet<openmittsu::protocol::GroupId> knownGroupsContainingMember;
	ASSERT_THROW(knownGroupsContainingMember = db->getKnownGroupsContainingMember(nonExistantContactId), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(knownGroupsContainingMember = db->getKnownGroupsContainingMember(contactIdC));
	ASSERT_EQ(2, knownGroupsContainingMember.size());
	ASSERT_TRUE(knownGroupsContainingMember.contains(groupA));
	ASSERT_TRUE(knownGroupsContainingMember.contains(groupC));
	ASSERT_NO_THROW(knownGroupsContainingMember = db->getKnownGroupsContainingMember(contactIdD));
	ASSERT_EQ(2, knownGroupsContainingMember.size());
	ASSERT_TRUE(knownGroupsContainingMember.contains(groupB));
	ASSERT_TRUE(knownGroupsContainingMember.contains(groupC));

	ASSERT_THROW(db->getGroupImage(nonExistantGroupId), openmittsu::exceptions::InternalErrorException);
	ASSERT_FALSE(db->getGroupImage(groupA).isAvailable());
	ASSERT_FALSE(db->getGroupImage(groupB).isAvailable());
	ASSERT_FALSE(db->getGroupImage(groupC).isAvailable());
	ASSERT_EQ(0, db->getMediaItemCount());

	QByteArray const testDataA(QStringLiteral("testDataA").toUtf8());
	openmittsu::protocol::MessageId const messageC = this->getFreeMessageId();
	ASSERT_THROW(db->storeReceivedGroupSetImage(nonExistantGroupId, nonExistantContactId, messageC, openmittsu::protocol::MessageTime::fromDatabase(123), openmittsu::protocol::MessageTime::fromDatabase(456), testDataA), openmittsu::exceptions::InternalErrorException);
	ASSERT_NO_THROW(db->storeReceivedGroupSetImage(groupC, contactIdD, messageC, openmittsu::protocol::MessageTime::fromDatabase(123), openmittsu::protocol::MessageTime::fromDatabase(456), testDataA));

	ASSERT_FALSE(db->getGroupImage(groupA).isAvailable());
	ASSERT_FALSE(db->getGroupImage(groupB).isAvailable());
	ASSERT_TRUE(db->getGroupImage(groupC).isAvailable());
	ASSERT_EQ(testDataA, db->getGroupImage(groupC).getData());
	ASSERT_EQ(2, db->getMediaItemCount());

	QByteArray const testDataB(QStringLiteral("longerTestDataB").toUtf8());
	openmittsu::protocol::MessageId const messageD = this->getFreeMessageId();
	ASSERT_NO_THROW(db->storeReceivedGroupSetImage(groupC, contactIdD, messageD, openmittsu::protocol::MessageTime::fromDatabase(789), openmittsu::protocol::MessageTime::fromDatabase(1011), testDataB));

	ASSERT_FALSE(db->getGroupImage(groupA).isAvailable());
	ASSERT_FALSE(db->getGroupImage(groupB).isAvailable());
	ASSERT_TRUE(db->getGroupImage(groupC).isAvailable());
	ASSERT_EQ(testDataB, db->getGroupImage(groupC).getData());
	ASSERT_EQ(3, db->getMediaItemCount());
}

TEST_F(DatabaseTestFramework, groupMessages) {
	openmittsu::protocol::ContactId contactIdB(QStringLiteral("BBBBBBBB"));
	openmittsu::crypto::KeyPair contactIdBKeyPair(openmittsu::crypto::KeyPair::randomKey());
	ASSERT_NO_THROW(db->storeNewContact(contactIdB, contactIdBKeyPair));
	ASSERT_TRUE(db->hasContact(contactIdB));

	openmittsu::protocol::GroupId groupA(contactIdB, 1);
	ASSERT_NO_THROW(db->storeNewGroup(groupA, { contactIdB, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupA));

	openmittsu::protocol::GroupId groupB(contactIdB, 2);
	ASSERT_NO_THROW(db->storeNewGroup(groupB, { contactIdB, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupB));

	openmittsu::protocol::GroupId groupC(contactIdB, 3);
	ASSERT_NO_THROW(db->storeNewGroup(groupC, { contactIdB, selfContactId }, false));
	ASSERT_TRUE(db->hasGroup(groupC));


	openmittsu::protocol::MessageId messageA(0);
	openmittsu::protocol::MessageId messageB(0);
	openmittsu::protocol::MessageId messageC(0);
	openmittsu::protocol::MessageId messageD(0);
	openmittsu::protocol::MessageId messageE(0);

	ASSERT_NO_THROW(messageA = db->storeSentGroupMessageText(groupA, openmittsu::protocol::MessageTime::fromDatabase(12345678), true, QStringLiteral("TestMessageA")));
	this->addMessageId(messageA);

	messageB = this->getFreeMessageId();
	ASSERT_NO_THROW(db->storeReceivedGroupMessageText(groupA, contactIdB, messageB, openmittsu::protocol::MessageTime::fromDatabase(12345), openmittsu::protocol::MessageTime::fromDatabase(12345678), QStringLiteral("TestMessageB")));

	ASSERT_NO_THROW(messageC = db->storeSentGroupMessageText(groupA, openmittsu::protocol::MessageTime::fromDatabase(23456789), true, QStringLiteral("TestMessageC")));
	this->addMessageId(messageC);

	openmittsu::utility::Location const location(1.2, 3.4, 5.6, QStringLiteral("Example Street 123, Somecity, Country"), QStringLiteral("Description"));
	ASSERT_NO_THROW(messageD = db->storeSentGroupMessageLocation(groupC, openmittsu::protocol::MessageTime::fromDatabase(12345678), true, location));
	this->addMessageId(messageD);
	QByteArray const imageData = QStringLiteral("SomeTestDataThatRepresentsTheImage").toUtf8();
	ASSERT_NO_THROW(messageE = db->storeSentGroupMessageImage(groupC, openmittsu::protocol::MessageTime::fromDatabase(23456789), true, imageData, QStringLiteral("An image caption")));
	this->addMessageId(messageE);

	openmittsu::protocol::ContactId nonExistantContactId(QStringLiteral("XXXXXXXX"));
	ASSERT_THROW(db->getMessageCursor(nonExistantContactId), openmittsu::exceptions::InternalErrorException);

	openmittsu::database::DatabaseGroupMessageCursor cursorB = db->getMessageCursor(groupA);
	openmittsu::database::DatabaseGroupMessageCursor cursorC = db->getMessageCursor(groupB);
	openmittsu::database::DatabaseGroupMessageCursor cursorD = db->getMessageCursor(groupC);
	ASSERT_FALSE(cursorB.isValid());
	ASSERT_FALSE(cursorC.isValid());
	ASSERT_FALSE(cursorD.isValid());

	// Seek all messages
	ASSERT_TRUE(cursorB.seek(messageA));
	ASSERT_TRUE(cursorB.seek(messageB));
	ASSERT_TRUE(cursorB.seek(messageC));
	ASSERT_FALSE(cursorB.seek(messageD));
	ASSERT_FALSE(cursorB.seek(messageE));

	ASSERT_FALSE(cursorC.seek(messageA));
	ASSERT_FALSE(cursorC.seek(messageB));
	ASSERT_FALSE(cursorC.seek(messageC));
	ASSERT_FALSE(cursorC.seek(messageD));
	ASSERT_FALSE(cursorC.seek(messageE));

	ASSERT_FALSE(cursorD.seek(messageA));
	ASSERT_FALSE(cursorD.seek(messageB));
	ASSERT_FALSE(cursorD.seek(messageC));
	ASSERT_TRUE(cursorD.seek(messageD));
	ASSERT_TRUE(cursorD.seek(messageE));

	// check seek First/Last
	ASSERT_TRUE(cursorB.seekToFirst());
	ASSERT_FALSE(cursorC.seekToFirst());

	ASSERT_TRUE(cursorB.isValid());
	ASSERT_FALSE(cursorC.isValid());

	ASSERT_FALSE(cursorB.previous());
	ASSERT_FALSE(cursorC.previous());

	ASSERT_TRUE((cursorB.getMessageId() == messageA) || (cursorB.getMessageId() == messageB));
	bool const wasMessageA = cursorB.getMessageId() == messageA;
	if (wasMessageA) {
		std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> message = cursorB.getMessage();
		ASSERT_EQ(message->getGroupId(), groupA);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageA"));
	} else {
		std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> message = cursorB.getMessage();
		ASSERT_EQ(message->getGroupId(), groupA);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageB"));
	}

	ASSERT_TRUE(cursorB.next());
	ASSERT_FALSE(cursorC.next());

	if (wasMessageA) {
		std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> message = cursorB.getMessage();
		ASSERT_EQ(messageB, cursorB.getMessageId());
		ASSERT_EQ(message->getGroupId(), groupA);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageB"));
	} else {
		std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> message = cursorB.getMessage();
		ASSERT_EQ(messageA, cursorB.getMessageId());
		ASSERT_EQ(message->getGroupId(), groupA);
		ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageA"));
	}

	ASSERT_TRUE(cursorB.next());
	ASSERT_EQ(messageC, cursorB.getMessageId());

	std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> message = cursorB.getMessage();
	ASSERT_EQ(message->getGroupId(), groupA);
	ASSERT_EQ(message->getContentAsText(), QStringLiteral("TestMessageC"));

	ASSERT_FALSE(cursorB.next());
	ASSERT_FALSE(cursorC.next());

	// and back again
	ASSERT_TRUE(cursorB.seekToFirst());
	ASSERT_FALSE(cursorC.seekToFirst());
	ASSERT_TRUE(cursorB.seekToLast());
	ASSERT_FALSE(cursorC.seekToLast());

	ASSERT_TRUE(cursorB.isValid());
	ASSERT_FALSE(cursorC.isValid());
	ASSERT_EQ(messageC, cursorB.getMessageId());

	{
		ASSERT_TRUE(cursorD.seek(messageD));
		std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> locationMessage = cursorD.getMessage();
		ASSERT_EQ(locationMessage->getGroupId(), groupC);
		ASSERT_EQ(locationMessage->getContentAsLocation(), location);

		ASSERT_TRUE(cursorD.next());
		std::shared_ptr<openmittsu::dataproviders::messages::GroupMessage> imageMessage = cursorD.getMessage();
		ASSERT_EQ(imageMessage->getGroupId(), groupC);
		ASSERT_TRUE(imageMessage->getContentAsImage().isAvailable());
		ASSERT_EQ(imageMessage->getContentAsImage().getData(), imageData);
	}
}

TEST_F(DatabaseTestFramework, settings) {
	QString const optionNameA = QStringLiteral("optionNameA");
	QString const optionValueA = QStringLiteral("12.345");
	QString const optionNameB = QStringLiteral("optionNameB");
	bool optionValueB = true;
	QString const optionNameC = QStringLiteral("optionNameC");
	QByteArray const optionValueC = QByteArray::fromHex(QStringLiteral("000102030405060708091011121314151617181920ff").toUtf8());

	ASSERT_FALSE(db->hasOption(optionNameA));
	ASSERT_FALSE(db->hasOption(optionNameB));
	ASSERT_FALSE(db->hasOption(optionNameC));

	ASSERT_NO_THROW(db->setOptionValue(optionNameA, optionValueA));

	ASSERT_TRUE(db->hasOption(optionNameA));
	ASSERT_FALSE(db->hasOption(optionNameB));
	ASSERT_FALSE(db->hasOption(optionNameC));

	ASSERT_NO_THROW(db->setOptionValue(optionNameB, optionValueB));
	ASSERT_NO_THROW(db->setOptionValue(optionNameC, optionValueC));
	
	ASSERT_TRUE(db->hasOption(optionNameA));
	ASSERT_TRUE(db->hasOption(optionNameB));
	ASSERT_TRUE(db->hasOption(optionNameC));

	QString optionValueAfterSaveA;
	bool optionValueAfterSaveB;
	QByteArray optionValueAfterSaveC;

	ASSERT_NO_THROW(optionValueAfterSaveA = db->getOptionValueAsString(optionNameA));
	ASSERT_NO_THROW(optionValueAfterSaveB = db->getOptionValueAsBool(optionNameB));
	ASSERT_NO_THROW(optionValueAfterSaveC = db->getOptionValueAsByteArray(optionNameC));

	ASSERT_EQ(optionValueA, optionValueAfterSaveA);
	ASSERT_EQ(optionValueB, optionValueAfterSaveB);
	ASSERT_EQ(optionValueC, optionValueAfterSaveC);
}
