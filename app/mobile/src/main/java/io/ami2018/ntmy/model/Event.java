package io.ami2018.ntmy.model;

import android.util.SparseArray;

import org.json.JSONException;
import org.json.JSONObject;

public class Event {

    private String name;
    private String description;
    private String start;
    private String end;
    private Integer eventId;
    private Integer creatorId;

    private Room room;
    private SparseArray<Category> categories;

    public Event(String name, String description, String start, String end, Integer eventId, Integer creatorId) {
        this.name = name;
        this.description = description;
        this.start = start;
        this.end = end;
        this.eventId = eventId;
        this.creatorId = creatorId;
        this.room = null;
        this.categories = new SparseArray<>();
    }

    public Event(JSONObject jsonObject) {
        try {
            this.name = jsonObject.getString("name");
            this.description = jsonObject.getString("description");
            this.start = jsonObject.getString("start");
            this.end = jsonObject.getString("end");
            this.eventId = jsonObject.getInt("eventID");
            this.creatorId = jsonObject.getJSONObject("creator").getInt("userID");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        this.room = null;
        this.categories = new SparseArray<>();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getStart() {
        return start;
    }

    public void setStart(String start) {
        this.start = start;
    }

    public String getEnd() {
        return end;
    }

    public void setEnd(String end) {
        this.end = end;
    }

    public Integer getEventId() {
        return eventId;
    }

    public void setEventId(Integer eventId) {
        this.eventId = eventId;
    }

    public Integer getCreatorId() {
        return creatorId;
    }

    public void setCreatorId(Integer creatorId) {
        this.creatorId = creatorId;
    }

    public SparseArray<Category> getCategories() {
        return categories;
    }

    public void setCategories(SparseArray<Category> categories) {
        this.categories = categories;
    }

    public Room getRoom() {
        return room;
    }

    public void setRoom(Room room) {
        this.room = room;
    }

    public void addCategory(Category category) {
        if (this.categories.get(category.getCategoryId()) == null) {
            this.categories.append(category.getCategoryId(), category);
        }
    }
}
