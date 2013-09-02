Octopress Commander (oc)
===================

## Octopress Site Management, Empowered.
OC is designed to be a jetpack for your Octopress site. With a powerful set of commands that provide an array of site management features, you should be able to focus on your content... instead of on shell commands.

## Example Workflow
### Create a New Post Markdown File 
##### (including) description + keyword + draft post YAML tags)
```
oc post "A really cool new post"
```

### Edit the Post + Watch Source for Changes + Launch Local Web Server
##### (with fuzzy search post launcher)
```
oc write "cool post"
```

### Publish the Post & git commit Your Source
##### (to GitHub Pages, Heroku, or Your Server of Choice with rsync)
```
oc publish
```