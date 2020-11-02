# Lab 04 - TelegramBot

from telegram.ext import Updater, CommandHandler, MessageHandler, Filters

import tasksapi


def start(bot, update):
    update.message.reply_text('Hi! Welcome to Todo Manager Bot.')


def wrong(bot, update):
    update.message.reply_text("I'm sorry, I can't do that.")


def show_tasks(bot, update):
    tasks = tasksapi.get_tasks()
    if len(tasks) == 0:
        update.message.reply_text("There are no tasks!")
    else:
        message = "Tasks inserted are:\n"
        for task in tasks:
            message += task + "\n"
        update.message.reply_text(message)


def new_task(bot, update, args):
    if len(args) > 0:
        new_task = " ".join(args)
        if tasksapi.add_task(new_task):
            update.message.reply_text("The new task was successfully added to the list!")
        else:
            update.message.reply_text("The new task was not inserted because it's already in the list.")
    else:
        update.message.reply_text("It seems you didn't provide a task.")


def remove_task(bot, update, args):
    if len(args) > 0:
        del_task = " ".join(args)
        if tasksapi.delete_task(del_task):
            update.message.reply_text("The task was successfully removed from the list!")
        else:
            update.message.reply_text("The task was not removed because it's not in the list.")
    else:
        update.message.reply_text("It seems you didn't provide a task.")


def remove_all_tasks(bot, update, args):
    if len(args) > 0:
        match_sub = " ".join(args)
        if tasksapi.delete_task_match(match_sub):
            update.message.reply_text("Tasks were successfully removed from the list!")
        else:
            update.message.reply_text("Tasks were not removed because an error occurred.")
    else:
        update.message.reply_text("It seems you didn't provide a string.")


def main():
    token = ""

    with open("token.txt") as tokenFile:
        token = tokenFile.read()
        tokenFile.close()

    updater = Updater(token)
    dp = updater.dispatcher
    dp.add_handler(CommandHandler("start", start))
    dp.add_handler(CommandHandler("showTasks", show_tasks))
    dp.add_handler(CommandHandler("newTask", new_task, pass_args=True))
    dp.add_handler(CommandHandler("removeTask", remove_task, pass_args=True))
    dp.add_handler(CommandHandler("removeAllTasks", remove_all_tasks, pass_args=True))

    dp.add_handler(MessageHandler(Filters.text, wrong))

    updater.start_polling()
    updater.idle()

    tasksapi.close_db()


if __name__ == "__main__":
    main()
