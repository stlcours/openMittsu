#include "src/dataproviders/MessageCenterThreadWorker.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/utility/Logging.h"
#include "src/utility/MakeUnique.h"
#include "src/utility/QObjectConnectionMacro.h"

namespace openmittsu {
	namespace dataproviders {

		MessageCenterThreadWorker::~MessageCenterThreadWorker() {
			//
		}

		bool MessageCenterThreadWorker::createMessageCenter(std::shared_ptr<openmittsu::widgets::TabController> const& tabController, std::shared_ptr<openmittsu::utility::OptionMaster> const& optionMaster) {
			std::shared_ptr<openmittsu::dataproviders::MessageCenter> newMessageCenter = std::make_shared<openmittsu::dataproviders::MessageCenter>(tabController, optionMaster);
			if (newMessageCenter) {
				m_messageCenter = newMessageCenter;

				return true;
			} else {
				LOGGER()->warn("Failed to create MessageCenter!");
				return false;
			}
		}

		bool MessageCenterThreadWorker::hasMessageCenter() const {
			return m_messageCenter != nullptr;
		}

		std::shared_ptr<openmittsu::dataproviders::MessageCenter> MessageCenterThreadWorker::getMessageCenter() {
			return m_messageCenter;
		}

		std::shared_ptr<openmittsu::dataproviders::MessageCenter> const& MessageCenterThreadWorker::getMessageCenter() const {
			return m_messageCenter;
		}

	}
}
