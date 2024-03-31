#pragma once
#include <boost/describe.hpp>
#include <string>

class ChatRoomModel
{
	BOOST_DESCRIBE_CLASS(ChatRoomModel, (), (), (RoomName, RoomDescription, RoomImageUrl), ())
public:
	ChatRoomModel() = default;
	~ChatRoomModel() = default;

	ChatRoomModel(const std::string& name, const std::string& description, const std::string& imageUrl) : RoomName(name), RoomDescription(description), RoomImageUrl(imageUrl)
	{
	}

	ChatRoomModel(const ChatRoomModel& other) : RoomName(other.RoomName), RoomDescription(other.RoomDescription), RoomImageUrl(other.RoomImageUrl)
	{
	}

	ChatRoomModel& operator=(const ChatRoomModel& other)
	{
		if (this == &other)
		{
			return *this;
		}
		RoomName = other.RoomName;
		RoomDescription = other.RoomDescription;
		RoomImageUrl = other.RoomImageUrl;

		return *this;
	}

	ChatRoomModel(ChatRoomModel&& other) noexcept : RoomName(std::move(other.RoomName)), RoomDescription(std::move(other.RoomDescription)), RoomImageUrl(std::move(other.RoomImageUrl))
	{
	}

	ChatRoomModel& operator=(ChatRoomModel&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		RoomName = std::move(other.RoomName);
		RoomDescription = std::move(other.RoomDescription);
		RoomImageUrl = std::move(other.RoomImageUrl);

		return *this;
	}

	std::string GetRoomName() const { return RoomName; }
	std::string GetRoomDescription() const { return RoomDescription; }
	std::string GetRoomImageUrl() const { return RoomImageUrl; }
	void SetRoomName(const std::string& name) { RoomName = name; }
	void SetRoomDescription(const std::string& description) { RoomDescription = description; }
	void SetRoomImageUrl(const std::string& imageUrl) { RoomImageUrl = imageUrl; }

protected:
	std::string RoomName = "";
	std::string RoomDescription = "";
	std::string RoomImageUrl = "";
};

